/*
* pcaModel.hpp
* Created on: 5/9/2024
* Author : Moin Shaikh
*/

#include"pcaModel.hpp"
#include"../logging/LoggerFactory.hpp"






#include<boost/lexical_cast.hpp>
#include<boost/algorithm/string.hpp>
#include<highfive/highfive.hpp>
#include<fstream>

using logging::LoggerFactory;
using cv::Mat;
using cv::Vec3f;
using boost::lexical_cast;
using boost::filesystem::path;
using std::string;
using std::vector;
using std::array;


namespace MorphableModels
{
	PcaModel::PcaModel()
	{
		engine.seed();
	}

	PcaModel PcaModel::loadStatismoModel(path h5file, PcaModel::ModelType modelType)
	{
		logging::Logger logger = LoggerFactory::Instance()->getLogger("morphalmodel");

		string logMessage("PcaModel: Cannot load a statismo model. Please re-run CMake with WITH_MORPHABLEMODEL_HDF5 set to ON.");
		logger.error(logMessage);
		throw std::runtime_error(logMessage);

		PcaModel model;

		// Load the shape or color model from the .h5 file
		string h5GroupType;
		if (modelType == ModelType::SHAPE) {
			h5GroupType = "shape";
		}
		else if (modelType == ModelType::COLOR) {
			h5GroupType = "color";
		}

		HighFive::File h5Model(h5file.string(), HighFive::File::ReadWrite);

		try {
			h5Model = HighFive::File(h5file.string(), HighFive::File::ReadOnly);
		}
		catch (const HighFive::Exception& e) {
			string errorMessage = "Could not open HDF5 file: " + string(e.what());
			logger.error(errorMessage);
			throw errorMessage;
		}

		// Load either the shape or texture mean
		string h5Group = "/" + h5GroupType + "/model";
		HighFive::Group modelReconstructive = h5Model.getGroup(h5Group);

		// Read the mean
		HighFive::DataSet dsMean = modelReconstructive.getDataSet("./mean");
		std::vector<size_t> dims = dsMean.getSpace().getDimensions();
		LoggerFactory::Instance()->getLogger("morphablemodel").debug("Dimensions of the model mean: " + lexical_cast<string>(dims[0]));
		model.mean = Mat(1, dims[0], CV_32FC1); // Use a row-vector, because of faster memory access and I'm not sure the memory block is allocated contiguously if we have multiple rows.
		dsMean.read(model.mean.ptr<float>(0));
		model.mean = model.mean.t(); // Transpose it to a col-vector


		// Read the eigenvalues
		dsMean = modelReconstructive.getDataSet("./pcaVariance");
		dims = dsMean.getSpace().getDimensions();
		LoggerFactory::Instance()->getLogger("morphablemodel").debug("Dimensions of the pcaVariance: " + lexical_cast<string>(dims[0]));
		model.eigenvalues = Mat(1, dims[0], CV_32FC1);
		dsMean.read(model.eigenvalues.ptr<float>(0));
		model.eigenvalues = model.eigenvalues.t();


		// Read the PCA basis matrix. It is stored normalized.
		dsMean = modelReconstructive.getDataSet("./pcaBasis");
		dims = dsMean.getSpace().getDimensions();
		LoggerFactory::Instance()->getLogger("morphablemodel").debug("Dimensions of the PCA basis matrix: " + lexical_cast<string>(dims[0]) + ", " + lexical_cast<string>(dims[1]));
		model.normalizedPcaBasis = Mat(dims[0], dims[1], CV_32FC1);
		dsMean.read(model.normalizedPcaBasis.ptr<float>(0));


		// Un-normalize the basis and store the unnormalized basis as well, i.e. multiply each eigenvector by 1 over its eigenvalue.
		model.unnormalizedPcaBasis = unnormalizePcaBasis(model.normalizedPcaBasis, model.eigenvalues);



		// Read the triangle-list
		string representerGroupName = "/" + h5GroupType + "/representer";
		HighFive::Group representerGroup = h5Model.getGroup(representerGroupName);
		dsMean = representerGroup.getDataSet("./reference-mesh/triangle-list");
		dims = dsMean.getSpace().getDimensions();
		LoggerFactory::Instance()->getLogger("morphablemodel").debug("Dimensions of the triangle-list: " + lexical_cast<string>(dims[0]) + ", " + lexical_cast<string>(dims[1]));
		Mat triangles(dims[0], dims[1], CV_32SC1);
		dsMean.read(triangles.ptr<int>(0));

		model.triangleList.resize(triangles.rows);
		for (unsigned int i = 0; i < model.triangleList.size(); ++i) {
			model.triangleList[i][0] = triangles.at<int>(i, 0);
			model.triangleList[i][1] = triangles.at<int>(i, 1);
			model.triangleList[i][2] = triangles.at<int>(i, 2);
		}

		// Load the landmarks mappings:
		// load the reference-mesh
		representerGroup = h5Model.getGroup(representerGroupName);
		dsMean = representerGroup.getDataSet("./reference-mesh/vertex-coordinates");
		dims = dsMean.getSpace().getDimensions();
		LoggerFactory::Instance()->getLogger("morphablemodel").debug("Dimensions of the reference-mesh vertex-coordinates matrix: " + lexical_cast<string>(dims[0]) + ", " + lexical_cast<string>(dims[1]));
		Mat referenceMesh(dims[0], dims[1], CV_32FC1);
		dsMean.read(referenceMesh.ptr<float>(0));


		// convert to 3 vectors with the x, y and z coordinates for easy searching
		vector<float> refx(referenceMesh.col(0).clone());
		vector<float> refy(referenceMesh.col(1).clone());
		vector<float> refz(referenceMesh.col(2).clone());

		// load the landmarks info (mapping name <-> reference (x, y, z)-coords)
		HighFive::Group landmarksGroup = h5Model.getGroup("/metadata/landmarks");
		dsMean = landmarksGroup.getDataSet("./text");

		std::string outputString;
		LoggerFactory::Instance()->getLogger("morphablemodel").debug("Reading landmark information from the model.");
		dsMean.read(outputString);


		vector<string> landmarkLines;
		boost::split(landmarkLines, outputString, boost::is_any_of("\n"), boost::token_compress_on);
		for (const auto& l : landmarkLines) {
			if (l == "") {
				continue;
			}
			vector<string> line;
			boost::split(line, l, boost::is_any_of(" "), boost::token_compress_on);
			string name = line[0];
			int visibility = lexical_cast<int>(line[1]);
			float x = lexical_cast<float>(line[2]);
			float y = lexical_cast<float>(line[3]);
			float z = lexical_cast<float>(line[4]);
			// Find the x, y and z values in the reference
			const auto ivx = std::find(begin(refx), end(refx), x);
			const auto ivy = std::find(begin(refy), end(refy), y);
			const auto ivz = std::find(begin(refz), end(refz), z);
			// TODO Check for .end()!
			const auto vertexIdX = std::distance(begin(refx), ivx);
			const auto vertexIdY = std::distance(begin(refy), ivy);
			const auto vertexIdZ = std::distance(begin(refz), ivz);
			// assert vx=vy=vz
			// Hmm this is not perfect. If there's another vertex where 1 or 2 coords are the same, it fails.
			// We should do the search differently: Find _all_ the vertices that are equal, then take the one that has the right x, y and z.
			model.landmarkVertexMap.insert(make_pair(name, vertexIdX));
		}


		return model;

	}

	PcaModel PcaModel::loadScmModel(path modelFileName, path landMarkVertexMappingFile, PcaModel::ModelType modelType)
	{
		logging::Logger logger = LoggerFactory::Instance()->getLogger("morphalmodel");
		PcaModel model;

		// Load the landmarks mappings
		/*
		std::ifstream ffpList;
		ffpList.open(landmarkVertexMappingFile.c_str(), std::ios::in);
		if (!ffpList.is_open()) {
			string errorMessage = "Error opening feature points file " + landmarkVertexMappingFile + ".";
			logger.error(errorMessage);
			throw std::runtime_error(errorMessage);
		}
		string line;
		while (ffpList.good()) {
			std::getline(ffpList, line);
			if (line == "" || line.substr(0, 2) == "//") { // empty line or starting with a '//'
				continue;
			}
			string currFfp; // Have a buffer string
			int currVertex = 0;
			std::stringstream ss(line); // Insert the string into a stream
			ss >> currFfp;
			ss >> currVertex;
			model.landmarkVertexMap.insert(make_pair(currFfp, currVertex));
			currFfp.clear();
		}
		ffpList.close();
		*/
		// Identity mapping: // TODO DO PROPER SOLUTION
		/*for (int i = 0; i < 28635; ++i)	{
			model.landmarkVertexMap.insert(make_pair(boost::lexical_cast<string>(i), i)); // maybe from 1 to ...
		}*/

		//Load Model;

		if (sizeof(unsigned int) != 4)
		{
			logger.warn("Warning: We're reading 4 Bytes from the file but sizeof(unsigned int) != 4. Check the code/behaviour.");
		}
		if (sizeof(double) != 8)
		{
			logger.warn("Warning: We're reading 8 Bytes from the file but sizeof(double) != 8. Check the code/behaviour.");
		}
		std::ifstream modelFile;
		modelFile.open(modelFileName.string(), std::ios::binary);
		if (!modelFile.is_open())
		{
			logger.warn("Could not open model file: " + modelFileName.string());
			exit(EXIT_FAILURE);
		}
		// Reading the shape model
		// Read (reference?) num triangles and vertices
		unsigned int numVertices = 0;
		unsigned int numTriangle = 0;

		modelFile.read(reinterpret_cast<char*> (&numVertices), 4);
		modelFile.read(reinterpret_cast<char*> (&numTriangle), 4);

		//Read Triangle 
		vector<array<int, 3>> triangleList;
		unsigned int v0, v1, v2;

		for (unsigned int i = 0; i < numTriangle; ++i)
		{
			v0 = v1 = v2 = 0;
			modelFile.read(reinterpret_cast<char*>(&v0), 4);
			modelFile.read(reinterpret_cast<char*>  (&v1), 4);
			modelFile.read(reinterpret_cast<char*> (&v2), 4);
			triangleList[i][0] = v0;
			triangleList[i][1] = v1;
			triangleList[i][2] = v2;

		}
		// Read number of rows and columns of the shape projection matrix (pcaBasis)
		unsigned int numShapePcaCoeffs = 0;
		unsigned int numShapeDims = 0;	// dimension of the shape vector (3*numVertices)
		modelFile.read(reinterpret_cast<char*>(&numShapePcaCoeffs), 4);
		modelFile.read(reinterpret_cast<char*>(&numShapeDims), 4);
		
		if(3*numVertices != numShapeDims)
		{
			logger.warn("Warning: Number of shape dimensions is not equal to three times the number of vertices. Something will probably go wrong during the loading.");
		}

		Mat unNormilizedPCABasisShape(numShapeDims, numShapePcaCoeffs, CV_32FC1);
		logger.debug("Loading PCA basis matrix with " + lexical_cast<string>(unNormilizedPCABasisShape.rows) + " rows and " + lexical_cast<string>(unNormilizedPCABasisShape.cols) + "cols.");
		for(unsigned int col = 0; col < numShapePcaCoeffs;++col)
		{
			for(unsigned int row = 0; row<numShapeDims;++row)
			{
				double var = 0.0;
				modelFile.read(reinterpret_cast<char*>(&var), 8);
				unNormilizedPCABasisShape.at<float>(row, col) = static_cast <float>(var);

			}
		}
		// Read mean shape vector
		unsigned int numMean = 0; // dimension of the mean (3*numVertices)
		modelFile.read(reinterpret_cast<char*>(&numMean), 4);
		if (numMean != numShapeDims) {
			logger.warn("Warning: Number of shape dimensions is not equal to the number of dimensions of the mean. Something will probably go wrong during the loading.");
		}
		Mat meanShape(numMean,1, CV_32FC1);
		unsigned int counter = 0; 
		double vd0, vd1, vd2;
		for(unsigned int i=0;i<numMean/3;++i)
		{
			vd0 = vd1 = vd2 = 0.0;
			modelFile.read(reinterpret_cast<char*>(&vd0), 8);
			modelFile.read(reinterpret_cast<char*>(&vd1), 8);
			modelFile.read(reinterpret_cast<char*>(&vd2), 8);
			meanShape.at<float>(counter, 0) = static_cast<float>(vd0);
			++counter;
			meanShape.at<float>(counter, 0) = static_cast<float>(vd1);
			++counter;
			meanShape.at<float>(counter, 0) = static_cast<float>(vd2);
			++counter;
		}
		//read eigen Value

		unsigned int numEigenValsShape = 0;
		modelFile.read(reinterpret_cast<char*>(&numEigenValsShape), 8);
		if (numEigenValsShape != numShapePcaCoeffs) {
			logger.warn("Warning: Number of coefficients in the PCA basis matrix is not equal to the number of eigenvalues. Something will probably go wrong during the loading.");
		}
		Mat eigenValueShape(numEigenValsShape, 1, CV_32FC1);
		for (unsigned int i = 0; i < numEigenValsShape; ++i) {
			double var = 0.0;
			modelFile.read(reinterpret_cast<char*>(&var), 8);
			eigenValueShape.at<float>(i, 0) = static_cast<float>(var); 
		}
		Mat normalizedPCABasisShape = normalizePcaBasis(unNormilizedPCABasisShape, eigenValueShape);
		if(modelType == ModelType::SHAPE)
		{
			model.mean = meanShape;
			model.unnormalizedPcaBasis = unNormilizedPCABasisShape;
			model.eigenvalues = eigenValueShape;
			model.triangleList = triangleList;
			modelFile.close();
			return model;
		}
		// Reading the color model
		// Read number of rows and columns of projection matrix
		unsigned int numTexturePcaCoeffs = 0;
		unsigned int numTextureDims = 0;
		modelFile.read(reinterpret_cast<char*>(&numTexturePcaCoeffs), 4);
		modelFile.read(reinterpret_cast<char*>(&numTextureDims), 4);
		// Read color projection matrix
		Mat unnormalizedPcaBasisColor(numTextureDims, numTexturePcaCoeffs, CV_32FC1);
		logger.debug("Loading PCA basis matrix with " + lexical_cast<string>(unnormalizedPcaBasisColor.rows) + " rows and " + lexical_cast<string>(unnormalizedPcaBasisColor.cols) + "cols.");
		for (unsigned int col = 0; col < numTexturePcaCoeffs; ++col) {
			for (unsigned int row = 0; row < numTextureDims; ++row) {
				double var = 0.0;
				modelFile.read(reinterpret_cast<char*>(&var), 8);
				unnormalizedPcaBasisColor.at<float>(row, col) = static_cast<float>(var);
			}
		}

		// Read mean color vector
		unsigned int numMeanColor = 0; // dimension of the mean (3*numVertices)
		modelFile.read(reinterpret_cast<char*>(&numMeanColor), 4);
		Mat meanColor(numMeanColor, 1, CV_32FC1);
		counter = 0;
		for (unsigned int i = 0; i < numMeanColor / 3; ++i) {
			vd0 = vd1 = vd2 = 0.0;
			modelFile.read(reinterpret_cast<char*>(&vd0), 8); // order in hdf5: RGB. Order in OCV: BGR. But order in vertex.color: RGB
			modelFile.read(reinterpret_cast<char*>(&vd1), 8);
			modelFile.read(reinterpret_cast<char*>(&vd2), 8);
			meanColor.at<float>(counter, 0) = static_cast<float>(vd0);
			++counter;
			meanColor.at<float>(counter, 0) = static_cast<float>(vd1);
			++counter;
			meanColor.at<float>(counter, 0) = static_cast<float>(vd2);
			++counter;
		}
		// Read color eigenvalues
		unsigned int numEigenValsColor = 0;
		modelFile.read(reinterpret_cast<char*>(&numEigenValsColor), 4);
		Mat eigenvaluesColor(numEigenValsColor, 1, CV_32FC1);
		for (unsigned int i = 0; i < numEigenValsColor; ++i) {
			double var = 0.0;
			modelFile.read(reinterpret_cast<char*>(&var), 8);
			eigenvaluesColor.at<float>(i, 0) = static_cast<float>(var);
		}
		// We read the unnormalized basis from the file. Now let's normalize it and store the normalized basis separately.
		Mat normalizedPcaBasisColor = normalizePcaBasis(unnormalizedPcaBasisColor, eigenvaluesColor);

		if (modelType == ModelType::COLOR) {
			model.mean = meanColor;
			model.unnormalizedPcaBasis = unnormalizedPcaBasisColor; // read from the file
			model.normalizedPcaBasis = normalizedPcaBasisColor;
			model.eigenvalues = eigenvaluesColor;
			model.triangleList = triangleList;

			modelFile.close();

			return model;
		}

		logger.error("Unknown ModelType, should never reach here.");
		//modelFile.close();
		//return model;
	}
	unsigned int PcaModel::getNumberOfPrincipalComponents() const
	{
		return normalizedPcaBasis.cols;
	}
	unsigned int PcaModel::getDataDimension() const
	{
		// Note: we could assert(normalizedPcaBasis.rows==unnormalizedPcaBasis.rows)
		return normalizedPcaBasis.rows;
	}


	std::vector<std::array<int, 3>> PcaModel::getTriangleList() const
	{
		return triangleList;
	}

	Mat PcaModel::getMean() const
	{
		return mean;
	}

	Vec3f PcaModel::getMeanAtPoint(string landmarkIdentifier) const
	{
		//int vertexId = landmarkVertexMap.at(landmarkIdentifier); // TODO hack. Do proper.
		int vertexId = boost::lexical_cast<int>(landmarkIdentifier);
		vertexId *= 3;
		if (vertexId >= mean.rows) {
			throw std::out_of_range("The given vertex id is larger than the dimension of the mean.");
		}
		return Vec3f(mean.at<float>(vertexId), mean.at<float>(vertexId + 1), mean.at<float>(vertexId + 2)); // we could use Vec3f(mean(Range(), Range())), maybe then we don't copy the data?
	}

	Vec3f PcaModel::getMeanAtPoint(unsigned int vertexIndex) const
	{
		vertexIndex *= 3;
		return Vec3f(mean.at<float>(vertexIndex), mean.at<float>(vertexIndex + 1), mean.at<float>(vertexIndex + 2));
	}

	Mat PcaModel::drawSample(float sigma/*=1.0f*/)
	{
		std::normal_distribution<float> distribution(0.0f, sigma); // TODO: c'tor takes the stddev. Update all the documentation!!!

		vector<float> alphas(getNumberOfPrincipalComponents());

		for (auto& a : alphas) {
			a = distribution(engine);
		}

		return drawSample(alphas);

		/* without calling drawSample(alphas): (maybe if we add noise)
		Mat alphas = Mat::zeros(getNumberOfPrincipalComponents(), 1, CV_32FC1);
		for (int row=0; row < alphas.rows; ++row) {
			alphas.at<float>(row, 0) = distribution(engine);
		}
		*/

		/* with noise: (does the noise make sense in drawSample(vector<float> coefficients)?)
		unsigned int vsize = mean.size();
		vector epsilon = Utils::generateNormalVector(vectorSize) * sqrt(m_noiseVariance);
		return m_mean + m_pcaBasisMatrix * coefficients + epsilon;
		*/
	}

	Mat PcaModel::drawSample(vector<float> coefficients)
	{
		Mat alphas(coefficients);
		/*
		Mat sqrtOfEigenvalues = eigenvalues.clone();
		for (unsigned int i = 0; i < eigenvalues.rows; ++i)	{
			sqrtOfEigenvalues.at<float>(i) = std::sqrt(eigenvalues.at<float>(i));
		}

		//Mat smallBasis = pcaBasis(cv::Rect(0, 0, 55, 100));
		//Mat smallMean = mean(cv::Rect(0, 0, 1, 100));

		//Mat modelSample = mean + pcaBasis * alphas.mul(sqrtOfEigenvalues); // Surr
		//Mat modelSample = mean + pcaBasis * alphas; // Bsl .h5 old
		*/
		// Not necessary anymore: We can now just do:
		Mat modelSample = mean + normalizedPcaBasis * alphas;

		return modelSample;
	}

	cv::Mat PcaModel::getNormalizedPcaBasis() const
	{
		return normalizedPcaBasis.clone();
	}

	cv::Mat PcaModel::getNormalizedPcaBasis(std::string landmarkIdentifier) const
	{
		//int vertexId = landmarkVertexMap.at(landmarkIdentifier); // Todo: Hacky
		int vertexId = boost::lexical_cast<int>(landmarkIdentifier); // Document behaviour. What to pass?
		vertexId *= 3;
		/*
		Mat sqrtOfEigenvalues = eigenvalues.clone();
		for (unsigned int i = 0; i < eigenvalues.rows; ++i)	{ // only do in case of Surrey model...
			sqrtOfEigenvalues.at<float>(i) = std::sqrt(eigenvalues.at<float>(i));
		}

		Mat unnormalizedBasis = pcaBasis.rowRange(vertexId, vertexId + 3).clone(); // We don't want to modify the original basis!

		// Normalise the basis:
		//Mat normalizedBasis = unnormalizedBasis * sqrtOfEigenvalues;
		for (int basis = 0; basis < unnormalizedBasis.cols; ++basis) {
			Mat normalizedBasis = unnormalizedBasis.col(basis).mul(sqrtOfEigenvalues.at<float>(basis));
			normalizedBasis.copyTo(unnormalizedBasis.col(basis));
		}

		return unnormalizedBasis; // now contains the normalised basis - we multiplied it with sqrt(evals) in the previous loop.
		*/
		// In the case where we don't have to normalise the basis, we could just do:
		return normalizedPcaBasis.rowRange(vertexId, vertexId + 3);
	}

	float PcaModel::getEigenvalue(unsigned int index) const
	{
		return eigenvalues.at<float>(index);
	}

	bool PcaModel::landmarkExists(std::string landmarkIdentifier) const
	{
		int vertexId = boost::lexical_cast<int>(landmarkIdentifier);
		vertexId *= 3;
		if (vertexId >= mean.rows) {
			return false;
		}
		else {
			return true;
		}
	}

	cv::Mat normalizePcaBasis(cv::Mat unnormalizedBasis, cv::Mat eigenvalues)
	{
		Mat normalizedPcaBasis(unnormalizedBasis.size(), unnormalizedBasis.type()); // empty matrix with the same dimensions
		Mat sqrtOfEigenvalues = eigenvalues.clone();
		for (unsigned int i = 0; i < eigenvalues.rows; ++i) {
			sqrtOfEigenvalues.at<float>(i) = std::sqrt(eigenvalues.at<float>(i));
		}
		// Normalize the basis: We multiply each eigenvector (i.e. each column) with the square root of its corresponding eigenvalue
		for (int basis = 0; basis < unnormalizedBasis.cols; ++basis) {
			Mat normalizedEigenvector = unnormalizedBasis.col(basis).mul(sqrtOfEigenvalues.at<float>(basis));
			normalizedEigenvector.copyTo(normalizedPcaBasis.col(basis));
		}

		return normalizedPcaBasis;
	}

	cv::Mat unnormalizePcaBasis(cv::Mat normalizedBasis, cv::Mat eigenvalues)
	{
		Mat unnormalizedBasis(normalizedBasis.size(), normalizedBasis.type()); // empty matrix with the same dimensions
		Mat oneOverSqrtOfEigenvalues = eigenvalues.clone();
		for (unsigned int i = 0; i < eigenvalues.rows; ++i) {
			oneOverSqrtOfEigenvalues.at<float>(i) = 1.0f / std::sqrt(eigenvalues.at<float>(i));
		}
		// De-normalize the basis: We multiply each eigenvector (i.e. each column) with 1 over the square root of its corresponding eigenvalue
		for (int basis = 0; basis < normalizedBasis.cols; ++basis) {
			Mat unnormalizedEigenvector = normalizedBasis.col(basis).mul(oneOverSqrtOfEigenvalues.at<float>(basis));
			unnormalizedEigenvector.copyTo(unnormalizedBasis.col(basis));
		}

		return unnormalizedBasis;
	}


}