/*
* SvmClassifier.cpp
* Created on : 07/ 12 / 2024
* Author : Moin Shaikh
*/
#define _CRT_SECURE_NO_WARNINGS

#include"SvmClassifier.hpp"
#include"HistogramIntersectionKernel.hpp"
#include"LinearKernel.hpp"
#include"PolynomialKernel.hpp"
#include"rbfKernel.hpp"
#include"../logging/LoggerFactory.hpp"

#ifndef WITH_MATLAB_CLASSIFIER
#include<matio.h>

#endif

#include<stdexcept>

using logging::Logger;
using logging::LoggerFactory;
using cv::Mat;
using std::pair;
using std::vector;
using std::make_pair;
using std::make_shared;
using std::invalid_argument;
using std::runtime_error;
using std::shared_ptr;
using std::string;

namespace Classification
{

	
	SvmClassifier::SvmClassifier(shared_ptr<kernel> Kernel) :
		VectorMachineClassifier(Kernel), supportVector(), coEffiencient()
	{

	}

	bool SvmClassifier::classify(const cv::Mat& featureVector) const
	{
		return classify(conputeHyperlaneDistance(featureVector));
	}

	pair<bool, double> SvmClassifier::getConfidence(const Mat& featureVector) const
	{
		return getConfidence(conputeHyperlaneDistance(featureVector));
	}

	bool SvmClassifier::classify(double hyperplaneDistance) const
	{
		return hyperplaneDistance >= threshold;
	}

	pair<bool, double> SvmClassifier::getConfidence(double HyperlaneDistance) const
	{
		if (classify(HyperlaneDistance))
			return make_pair(true, HyperlaneDistance);
		else
			return make_pair(false, -HyperlaneDistance);
	}
	double SvmClassifier::conputeHyperlaneDistance(const cv::Mat& featureVector) const
	{
		double Distance = -bias;
		for (size_t i = 0; i < supportVector.size(); ++i)
		{
			Distance += coEffiencient[i] * Kernel->compute(featureVector, supportVector[i]);
		}
		return Distance;
	}
	void SvmClassifier::setSvmParameters( const vector<Mat>& supportVectors, const vector<float>& coEfficients, const double& bias) {
		this->supportVector = supportVectors;
		this->coEffiencient = coEfficients;
		this->bias = bias;
	}

	void SvmClassifier::store(std::ofstream& file)
	{
		if (!file)
		{
			throw invalid_argument("SVM classifier: file is not open");
		}
		file << "Kernel ";
		if (dynamic_cast<LinearKernel*> (getKernel().get()))
		{
			file << "Linear\n";
		}
		else if (PolynomialKernel* Kernel = dynamic_cast<PolynomialKernel*>(getKernel().get()))
		{
			file << "Polynomial " << Kernel->getDegree() << ' ' << Kernel->getConstant() << ' ' << Kernel->getAlpha() << '\n';
		}
		else if (RbfKernel* Kernel = dynamic_cast<RbfKernel*> (getKernel().get()))
		{
			file << "RBF " << Kernel->getGamma() << "\n";
		}
		else if (dynamic_cast<histogramIntersectionKernel*> (getKernel().get()))
		{
			file << "HIK\n";
		}
		else
		{
			throw runtime_error("SVM CLASSIFIER: Unknown KERNEL Type");
		}
		file << "Bias " << getBias() << "\n";
		file << "Coefficients " << getCoEffienct().size() << "\n";
		for(float coEfficient : getCoEffienct())
		{
			file << coEfficient<<"\n";
			const cv::Mat &vector = getSupportVector().front();
			int count = getSupportVector().size();
			int rows = vector.rows;
			int column = vector.cols;
			int channels = vector.channels();
			int depth = vector.depth();
			file<<"SupportVector "<< count << ' ' << rows << ' ' << column << ' ' << channels << ' ' << depth << '\n';
			switch(depth)
			{
			case (CV_8U): storeSupportVector<uchar>(file, getSupportVector()); 
				break;
			case(CV_32S): storeSupportVector<int32_t>(file, getSupportVector());
				break;
			case(CV_32F): storeSupportVector<float>(file, getSupportVector());
				break;
			case(CV_64F): storeSupportVector<double>(file, getSupportVector());
				break;
			defualt:throw runtime_error("SvmClassifier: cannot store support vectors of depth other than CV_8U, CV_32S, CV_32F or CV_64F");
			}

		}

	}

	shared_ptr<SvmClassifier> SvmClassifier::load(std::ifstream &file)
	{
		if(!file)
		{
			throw invalid_argument("SVM Classifier: Can not read from stream");
		}
		string temp;
		file >> temp; //Kernel

		shared_ptr<kernel> Kernel;
		string KernelType;
		file >> KernelType;
		if(KernelType == "Linear")
		{
			Kernel.reset(new LinearKernel());
		}
		else if(KernelType == "Polynomial")
		{
			int degree;
			double scale, constant;
			file >> degree >> constant >> scale;
			Kernel.reset(new PolynomialKernel(constant, scale, degree));
		}
		else if(KernelType == "RBF")
		{
			double gamma;
			file >> gamma;
			Kernel.reset(new RbfKernel(gamma));
		}
		else if(KernelType == "HIK")
		{
			//Kernel.reset(new HistogramIntersectionKernel());
		}
		else
		{
			throw runtime_error("SVM CLASSIFIER: Unknown KERNEL Type");
		}

		shared_ptr<SvmClassifier> svm = make_shared<SvmClassifier>(Kernel);
		size_t count;
		file >> temp; // "Coefficients"
		file >> count;
		svm->coEffiencient.resize(count);
		for (size_t i = 0; i < count; ++i)
		{
			file >> svm->coEffiencient[i];
		}
			

		int rows, cols, channels, depth;
		file >> temp; // "SupportVectors"
		file >> count; // should be the same as above
		file >> rows;
		file >> cols;
		file >> channels;
		file >> depth;
		switch (depth) {
		case CV_8U: loadSupportVector<uchar>(file, count, rows, cols, channels, depth, svm->supportVector); 
			break;
		case CV_32S: loadSupportVector<int32_t>(file, count, rows, cols, channels, depth, svm->supportVector); 
			break;
		case CV_32F: loadSupportVector<float>(file, count, rows, cols, channels, depth, svm->supportVector);
			break;
		case CV_64F: loadSupportVector<double>(file, count, rows, cols, channels, depth, svm->supportVector); 
			break;
		default: throw runtime_error(
			"SvmClassifier: cannot load support vectors of depth other than CV_8U, CV_32S, CV_32F or CV_64F");
		}
		return svm;


	}

	shared_ptr<SvmClassifier> SvmClassifier::loadFromText(const string& classifierFileName)
	{
		Logger logger = LoggerFactory::Instance()->getLogger("Classification");
		logger.info("Loading Svm Classifier from text file");
		
		std::ifstream file(classifierFileName.c_str());
		if(!file.is_open())
		{
			throw runtime_error("Svm Classifier : Invalid Classifier File");
		}
		string line;
		if(!getline(file, line))
		{
			throw runtime_error("Svm Classifier: Invalid Classifier File");
		}


		//read Kernel parameter

		shared_ptr<kernel> Kernel;
		std::istringstream lineStream(line);
		if(lineStream.good() && !lineStream.fail())
		{
			string KernelType;
			lineStream >> KernelType;
			if(KernelType != "FullPolynomial")
			{
				throw runtime_error("Svm Classifier: Unkown Kernel Type" + KernelType);
			}
			int degree;
			double constant, scale;
			lineStream >> degree >> constant >> scale;
			Kernel.reset(new PolynomialKernel(constant, scale, degree));
		}

		shared_ptr<SvmClassifier> svm = make_shared<SvmClassifier>(Kernel);
		int svCount;
		if (!std::getline(file, line))
		{
			throw runtime_error("Svm Classifier: Invalid Classifier File");
		}
		std::sscanf(line.c_str(), "Number of SV: %d", &svCount);

		int dimentionCount;
		if (!std::getline(file, line))
		{
			throw runtime_error("SvmClassifier: Invalid classifier file");
		}
		std::sscanf(line.c_str(), "Dim of SV : %d", &dimentionCount);
		float bias;
		if (!std::getline(file, line))
		{
			throw runtime_error("SvmClassifier: Invalid classifier file");
		}
		std::sscanf(line.c_str(), "B0 : %f", &bias);
		svm->bias = bias;

		//CoEfficient
		svm->coEffiencient.resize(svCount);
		for (int i = 0; i < svCount; ++i)
		{
			float alpha;
			int index;
			if (!std::getline(file, line))
			{
				throw runtime_error("SvmClassifier: Invalid classifier file");
			}
			std::sscanf(line.c_str(), "alphas[%d]=%f", &index, &alpha);
			svm->coEffiencient.at(index) = alpha;

		}
		//read Containing Support Vectors Machine
		if(!std::getline(file,line))
		{
			throw runtime_error("Svm Classifier: Invalid classifier file ");
		}
		//read Support Vectors;
		svm->supportVector.reserve(svCount);
		for(int i=0;i<svCount;++i)
		{
			Mat vector(1, dimentionCount, CV_32F);
			if(!std::getline(file,line))
			{
				throw runtime_error("SvmClassifier: Invalid classifier file");
			}
			float* values = vector.ptr<float>(0);
			for(int j=0; j< dimentionCount;++j)
			{
				lineStream >> values[j];
			}
			svm->supportVector.emplace_back(vector);
		}
		// TODO: Note: We never close the file?
		logger.info("SVM successfully read.");
		return svm;
	}

	shared_ptr<SvmClassifier> SvmClassifier::loadFromMATLAB(const string& classifierFileName)
	{
		Logger logger = LoggerFactory::Instance()->getLogger("Classification");
		#ifndef WITH_MATLAB_CLASSIFIER
		logger.info("Loading SVM classifier from Matlab file: " + classifierFileName);
		mat_t* pMatFile;
		matvar_t* pMxArray; //Mat 
		double* matdata;

		pMatFile = Mat_Open(classifierFileName.c_str(), MAT_ACC_RDONLY);
		if (pMatFile == NULL)
		{ 
			throw invalid_argument("SvmClassifier: Could not open the provided classifier filename: " + classifierFileName); 
		}

		pMxArray = Mat_VarRead(pMatFile, "param_nonlin1"); 
		if (pMxArray == NULL) 
		{
			throw runtime_error("SvmClassifier: There is no param_nonlin1 in the classifier file."); 
		}

		matdata = static_cast<double*>(pMxArray->data);
		float nonlinThreshold = static_cast<float>(matdata[0]);
		int nonLinType = static_cast<int>(matdata[1]); 
		float basisParam = static_cast<float>(matdata[2] / 65025.0); 
		int polyPower = static_cast<int>(matdata[3]); 
		float divisor = static_cast<float>(matdata[4]); 
		Mat_VarFree(pMxArray);

		shared_ptr<kernel> Kernel;
		if (nonLinType == 1) { // polynomial kernel
			Kernel.reset(new PolynomialKernel(1 / divisor, basisParam / divisor, polyPower));
		}
		else if (nonLinType == 2) { // RBF kernel
			Kernel.reset(new RbfKernel(basisParam));
		}
		else {
			throw runtime_error("SvmClassifier: Unsupported kernel type. Currently, only polynomial and RBF kernels are supported.");
			// TODO We should also throw/print the unsupported nonLinType value to the user
		}
		
		if (nonLinType == 1) 
		{ 
			Kernel.reset(new PolynomialKernel(1 / divisor, basisParam / divisor, polyPower));
		}
		else if (nonLinType == 2) 
		{ 
			Kernel.reset(new RbfKernel(basisParam));
		}
		else
		{ 
			throw runtime_error("SvmClassifier: Unsupported kernel type. Currently, only polynomial and RBF kernels are supported."); 
		} 
		shared_ptr<SvmClassifier> svm = make_shared<SvmClassifier>(Kernel);

		svm->bias = nonlinThreshold; pMxArray = Mat_VarRead(pMatFile, "support_nonlin1"); 

		if (pMxArray == NULL) 
		{ 
			throw runtime_error("SvmClassifier: There is a nonlinear SVM in the file, but the matrix support_nonlin1 is lacking.");
		} 

		if (pMxArray->rank != 3) 
		{ 
			throw runtime_error("SvmClassifier: The matrix support_nonlin1 in the file should have 3 dimensions."); 
		}
		const size_t* dim = pMxArray->dims; 

		int numSV = static_cast<int>(dim[2]);
		matdata = static_cast<double*>(pMxArray->data);
		int filter_size_x = static_cast<int>(dim[1]);
		int filter_size_y = static_cast<int>(dim[0]);

		svm->supportVector.reserve(numSV);
		svm->coEffiencient.reserve(numSV);
		int k = 0;

		for (int sv = 0; sv < numSV; ++sv)
		{ 
			Mat supportVector(filter_size_y, filter_size_x, CV_8U);
			uchar* values = supportVector.ptr<uchar>(0);
			for (int x = 0; x < filter_size_x; ++x)
			{ 
				for (int y = 0; y < filter_size_y; ++y)
				{ 
					values[y * filter_size_x + x] = static_cast<uchar>(255.0 * matdata[k++]);
				} 
			}
			svm->supportVector.emplace_back(supportVector); 
		} 
		Mat_VarFree(pMxArray);
		pMxArray = Mat_VarRead(pMatFile, "weight_nonlin1");

		if (pMxArray == NULL)
		{ 
			throw runtime_error("SvmClassifier: There is a nonlinear SVM in the file but the matrix weight_nonlin1 is lacking."); 
		}

		matdata = static_cast<double*>(pMxArray->data);

		for (int sv = 0; sv < numSV; ++sv)
		{ 
			svm->coEffiencient.emplace_back(static_cast<float>(matdata[sv])); 
		}

		Mat_VarFree(pMxArray); if (Mat_Close(pMatFile) != 0) 
		{
			logger.warn("SvmClassifier: Could not close file " + classifierFileName); 
		} 
		logger.info("SVM successfully read.");

		return svm;

		#endif // !WITH_MATLAB_CLASSIFIER

	}
}