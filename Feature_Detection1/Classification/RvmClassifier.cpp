/*
* RvmClassifier.cpp
* Created on :14/ 12 / 2024
* Author : Moin Shaikh
*/

#include"RvmClassifier.hpp"
#include"RvmProbabilisticClassifier.hpp"
#include"PolynomialKernel.hpp"
#include"RbfKernel.hpp"
#include"../logging/LoggerFactory.hpp"
#ifdef WITH_MATLAB_CLASSIFIER

#endif
#include<matio.h>
#include<stdexcept>
#include<fstream>
#include<boost/filesystem/path.hpp>
#include<boost/lexical_cast.hpp>

using boost::filesystem::path;
using boost::lexical_cast;
using boost::property_tree::ptree;
using std::pair;
using logging::Logger;
using logging::LoggerFactory;
using cv::Mat;
using std::make_pair;
using std::vector;
using std::string;
using std::shared_ptr;
using std::invalid_argument;
using std::runtime_error;
using std::logic_error;



namespace Classification
{
	RvmClassifier::RvmClassifier(std::shared_ptr<kernel> Kernel,bool cascadeCoEfficient) : VectorMachineClassifier(Kernel), supportVector(),coEfficients()
	{
		// TODO coefficients() probably different... vector<vector<... how?

		//useCaching = !cascadedCoefficients;
	}
	RvmClassifier::~RvmClassifier() {}

	bool RvmClassifier::classify(const Mat&featureVector) const
	{
		return classify(computeHyperPlaneDistance(featureVector));
	}

	pair<bool, double> RvmClassifier::getConfidence(const Mat& featureVector) const
	{
		return getConfidence(computeHyperPlaneDistance(featureVector));
	}

	pair<int, double> RvmClassifier::getConfidence(std::pair<int, double> levelAndDistance) const
	{

		if(classify(levelAndDistance))
		{
			return make_pair(true, levelAndDistance.second);
		}
		else
		{
			return make_pair(false, -levelAndDistance.second);
		}

	}

	bool RvmClassifier::classify(pair<int, double> levelAndDistance) const
	{
		// TODO the following todo was moved here from the end of the getHyperplaneDistance function (was in classify before)
		// TODO: filter statistics, nDropedOutAsNonFace[filter_level]++;
		// We ran till the REAL LAST filter (not just the numUsedFilters one), save the certainty
		int filter_Level = levelAndDistance.first;
		double hyperLaneDistance = levelAndDistance.second;
		return filter_Level + 1 == this->numFilterToUse && hyperLaneDistance >= this->hierarchicalThresholds[filter_Level];
	}

	pair<int, double> RvmClassifier::computeHyperPlaneDistance(const Mat& featureVector) const
	{
		int filterLevel = -1;
		double hyperPlaneDistance = 0;
		vector<double> filterEvalCache(this->numFilterToUse);
		do
		{
			++filterLevel;
			hyperPlaneDistance = computeHyperPlaneDistanceCached(featureVector, filterLevel, filterEvalCache);
		} while (hyperPlaneDistance >= hierarchicalThresholds.at(filterLevel) && filterLevel + 1 < this->numFilterToUse);
		return make_pair(filterLevel, hyperPlaneDistance);
	}

	double RvmClassifier::computeHyperPlaneDistance(const Mat&featureVector,const size_t &filterLevel) const
	{
		double distance = -bias;

		for(size_t i=0;i<=filterLevel;++i)
		{
			distance += coEfficients.at(filterLevel).at(i) + Kernel->compute(featureVector, supportVector.at(i));
		}
		return distance;

	}

	double RvmClassifier::computeHyperPlaneDistanceCached(const Mat &featureVector,const size_t &filterLevel,vector<double> &filterEvalCache) const
	{
		if(filterEvalCache.size() == filterLevel && filterLevel != 0)
		{
			double distance = filterEvalCache.at(filterLevel - 1);
			distance += coEfficients.at(filterLevel).at(filterLevel) * Kernel->compute(featureVector, supportVector.at(filterLevel));
			filterEvalCache.emplace_back(distance);
			return distance;
		}
		filterEvalCache.clear();

		double distance = -bias;
		for(size_t i=0;i<=filterLevel;++i)
		{
			distance += coEfficients.at(filterLevel).at(i) * Kernel->compute(featureVector, supportVector.at(i));
		}
		filterEvalCache.emplace_back(distance);
		return distance;
	}

	unsigned int RvmClassifier::getNumFilterToUse(void) const
	{
		return numFilterToUse;
	}

	void RvmClassifier::setNumfilterToUse(const unsigned int numFilter)
	{
		if (numFilter == 0 || numFilter > this->coEfficients.size())
		{
			numFilterToUse = this->coEfficients.size();
		}
		else
		{
			numFilterToUse = numFilter;
		}
	}
	shared_ptr<RvmClassifier> RvmClassifier::load(const ptree& subTree)
	{
		path classifierFile = subTree.get<path>("classifierFile");
		if(classifierFile.extension()==".mat")
		{
			shared_ptr<RvmClassifier> rvm = loadFromMATLAB(classifierFile.string(), subTree.get<string>("threshold"));
			// Do some stuff, e.g.
			//int numFiltersToUse = subtree.get<int>("");
			//Number filters to use
			//wvm->numUsedFilters=280;	// Todo make dynamic (from script)
			return rvm;
		}
		else 
		{
			throw logic_error("ProbabilisticRvmClassifier: Only loading of .mat RVMs is supported. If you want to load a non-cascaded RVM, use an SvmClassifier.");
		}
	}
    
	shared_ptr<RvmClassifier> RvmClassifier::loadFromMATLAB(const string& classifierFile, const string& thresholdFile)
	{
		Logger logger = LoggerFactory::Instance()->getLogger("Classification");


		logger.info("Loading Rvm classifier from MATLAB file:" + classifierFile);
		mat_t* pmatFile;
		matvar_t* pMXArray;
		
		pmatFile = Mat_Open(classifierFile.c_str(), MAT_ACC_RDONLY);
		if (pmatFile == 0)
		{
			throw std::invalid_argument("Rvm Classifier: could not Open Provided classifier file " + classifierFile);
		}
		pMXArray = Mat_VarRead(pmatFile, "num_hk");
		if (pMXArray == 0)
		{
			throw std::runtime_error("Rvm Classifier: There is no num_hk in the classifier file.");
		}
		int numFilter = *(int*)pMXArray->data;
		Mat_VarFree(pMXArray);
		logger.debug("Found " + boost::lexical_cast<std::string>(numFilter) + " reduced set vectors (RSVs).");

		float nonlinThreshold;
		int nonLinType;
		float basisParam;
		int polyPower;
		float divisor;

		pMXArray = Mat_VarRead(pmatFile, "param_nonlin1_rvm");
		if (pMXArray != NULL)
		{
			double* matdata = (double*)pMXArray->data;
			nonlinThreshold = (float)matdata[0];
			nonLinType = (int)matdata[1];
			basisParam = (float)(matdata[2] / 65025.0);
			polyPower = (int)matdata[3];
			divisor = (float)matdata[4];
			Mat_VarFree(pMXArray);
		}
		else
		{
			pMXArray = Mat_VarRead(pmatFile, "param_nonlin1");
			if (pMXArray != NULL) {
				double* matdata = (double*)pMXArray->data;
				nonlinThreshold = (float)matdata[0];
				nonLinType = (int)matdata[1];
				basisParam = (float)(matdata[2] / 65025.0);
				polyPower = (int)matdata[3];
				divisor = (float)matdata[4];
				Mat_VarFree(pMXArray);
			}
			else
			{
				throw std::runtime_error("Rvm Classifier: There is no param_nonlin1 in the classifier file.OR  Could not find kernel parameters and bias.");
			}
		}
		std::shared_ptr<kernel> Kernel;
		if (nonLinType == 1)
		{
			Kernel.reset(new PolynomialKernel(1 / divisor, basisParam / divisor, polyPower));
		}
		else if(nonLinType == 2)
		{
			Kernel.reset(new RbfKernel(basisParam));
		}
		else
		{
			throw runtime_error("RvmClassifier: Could not find kernel parameters and bias.");
		}
		
		shared_ptr<RvmClassifier> rvm = std::make_shared<RvmClassifier>(Kernel);
		rvm->bias = nonlinThreshold;
		logger.debug("Reading the " + boost::lexical_cast<std::string>(numFilter) + " non-linear filters support_hk* and weight_hk* ...");

		char str[100];
		sprintf_s(str, "support_hk", 1);
		pMXArray = Mat_VarRead(pmatFile, str);

		if (pMXArray == NULL) 
		{
			throw std::runtime_error("RvmClassifier: Unable to find the matrix 'support_hk1' in the classifier file.");
		}

		if (pMXArray->rank != 2)
		{
			throw std::runtime_error("RvmClassifier: The matrix 'support_hk1' in the classifier file should have 2 dimensions.");
		}

		int filterSizeY = (int)pMXArray->dims[0];
		int filterSizeX = (int)pMXArray->dims[1];
		Mat_VarFree(pMXArray);

		rvm->supportVector.reserve(numFilter);
		rvm->coEfficients.reserve(numFilter);
		

		for (int i = 0; i < numFilter; ++i)
		{
			sprintf_s(str, "support_hk%d", i + 1);
			pMXArray = Mat_VarRead(pmatFile, str);


			if (pMXArray == NULL)
			{
				throw std::runtime_error("RvmClassifier: Unable to find the matrix 'support_hk" + boost::lexical_cast<std::string>(i + 1) + "' in the classifier file.");
			}

			if (pMXArray->rank != 2)
			{
				throw std::runtime_error("RvmClassifier: The matrix 'support_hk" + boost::lexical_cast<std::string>(i + 1) + "' in the classifier file should have 2 dimensions.");
			}


			double *matdata = (double*)pMXArray->data;
			int k = 0;
			Mat supportVector(filterSizeY, filterSizeX, CV_32F);
			float* values = supportVector.ptr<float>(0);

			for (int x = 0; x < filterSizeX; ++x)
			{
				for (int y = 0; y < filterSizeY; ++y)
				{
					values[y * filterSizeX + x] = static_cast<float>(matdata[k++]);
				}
			}
			rvm->supportVector.emplace_back(supportVector);
			Mat_VarFree(pMXArray);


			sprintf_s(str, "weight_hk%d",i+1);
			pMXArray = Mat_VarRead(pmatFile, str);

			if (pMXArray != NULL)
			{
				if ((pMXArray->dims[1] != i + 1) && (pMXArray->dims[0] != i + 1))
				{
					throw std::runtime_error("RvmClassifier: The matrix " + boost::lexical_cast<std::string>(str) + " in the classifier file should have dimensions 1x" + boost::lexical_cast<std::string>(i + 1) + " or " + boost::lexical_cast<std::string>(i + 1) + "x1");
				}
			}
			std::vector<float> coefficientsForFilter;
			//matData = (double*)pMXArray->data;
			for (int j = 0; j <= i; ++j) {
				coefficientsForFilter.emplace_back(static_cast<float>(matdata[j]));
			}
			rvm->coEfficients.emplace_back(coefficientsForFilter);
			Mat_VarFree(pMXArray);
		}

		logger.debug("Vectors and weights successfully read.");

		if (Mat_Close(pmatFile) != 0)
		{
			logger.warn("RvmClassifier: Could not close file " + classifierFile);
		}
		logger.info("RVM successfully read.");

		logger.info("Loading RVM thresholds from Matlab file: " + thresholdFile);
		pmatFile = Mat_Open(thresholdFile.c_str(), MAT_ACC_RDONLY);

		if (pmatFile == NULL)
		{
			throw std::runtime_error("RvmClassifier: Unable to open the thresholds file (wrong format?):" + thresholdFile);
		}
		else
		{
			pMXArray = Mat_VarRead(pmatFile, "hierar_thresh");
			double* matdata = (double*)pMXArray->data;
			for (int o = 0; o < (int)pMXArray->dims[1]; ++o)
			{
				rvm->hierarchicalThresholds.emplace_back(static_cast<float>(matdata[o]));
			}
			Mat_VarFree(pMXArray);
		}
		Mat_Close(pmatFile);
		if (rvm->hierarchicalThresholds.size() != rvm->coEfficients.size())
		{
			throw std::runtime_error("RvmClassifier: Something seems to be wrong, hierarchicalThresholds.size() != coefficients.size(): " + boost::lexical_cast<std::string>(rvm->hierarchicalThresholds.size()) + "!=" + boost::lexical_cast<std::string>(rvm->coEfficients.size()));
		}
		logger.info("RVM thresholds successfully read.");
		rvm->setNumfilterToUse(numFilter);
		return rvm;
	
	string errorMessage("RvmClassifier: Cannot load a Matlab classifier, library compiled without support for Matlab. Please re-run CMake with WITH_MATLAB_CLASSIFIER enabled.");
	logger.error(errorMessage);
	throw std::runtime_error(errorMessage);

	
	}




		
		
	
}
    



