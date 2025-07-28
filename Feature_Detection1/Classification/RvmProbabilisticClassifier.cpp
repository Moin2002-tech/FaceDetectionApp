/*
* ProbabilisticRvmClassifier.hpp
* Created on : 03/ 12 / 2024
* Author : Moin Shaikh
*/


#include"RvmProbabilisticClassifier.hpp"
#include"RvmClassifier.hpp"
#include"../logging/LoggerFactory.hpp"


#include<matio.h>



//#include<boost/filesystem/path.hpp>
#include<boost/filesystem/path.hpp>
#include<iostream>
#include<stdexcept>


using logging::Logger;
using logging::LoggerFactory;
using cv::Mat;
using boost::filesystem::path;
using boost::property_tree::ptree;

using std::pair;
using std::string;
using std::make_pair;
using std::shared_ptr;
using std::make_shared;
using std::invalid_argument;
using std::logic_error;
using std::runtime_error;


namespace Classification
{
	ProbabilisticRvmClassifier::ProbabilisticRvmClassifier() :
		rvm(), logisticA(0), logisticB(0){}

	ProbabilisticRvmClassifier::ProbabilisticRvmClassifier(shared_ptr<RvmClassifier> rvm, double logisticA, double logisticB) :
		rvm(rvm), logisticA(logisticA), logisticB(logisticB)
	{}

	bool ProbabilisticRvmClassifier::classify(const Mat &featureVector) const
	{
		return rvm->classify(featureVector);
	}

	pair<bool, double> ProbabilisticRvmClassifier::getConfidence(const Mat& featureVector) const
	{
		return rvm->getConfidence(featureVector);
	}

	pair<bool, double> ProbabilisticRvmClassifier::getProbability(const Mat& featureVector) const
	{
		return getProbability(rvm->computeHyperPlaneDistance(featureVector));
	}

	pair<bool, double> ProbabilisticRvmClassifier::getProbability(pair<int, double> levelAndDistance) const
	{
		// Do sigmoid stuff:
		// NOTE MOIN: Here we calculate the probability for all RVM patches, also of those that
		//      did not run up to the last filter. Those probabilities are wrong, but for the face-
		//      detector it seems to work out ok. However, for the eye-detector, the probability-maps
		//      don't look good. See below for the code.
		double probability = 1.0f / (1.0f + exp(logisticA + logisticB * levelAndDistance.second));
		return make_pair(rvm->classify(levelAndDistance), probability);
	}

	void ProbabilisticRvmClassifier::setLogisticParameter(double logisticA,double logisticB)
	{
		this->logisticA = logisticA;
		this->logisticB = logisticB;
	}

	shared_ptr<ProbabilisticRvmClassifier> ProbabilisticRvmClassifier::load(ptree& subtree)
	{
		path classifierFile = subtree.get<path>("classifierFile");
		if(classifierFile.extension() == ".mat")
		{
			pair<double, double> logisticParameters = ProbabilisticRvmClassifier::loadSigmoidParamsFromMATLAB(subtree.get<path>("thresholdsFile").string());
			shared_ptr<RvmClassifier> rvm = RvmClassifier::load(subtree);
			return make_shared<ProbabilisticRvmClassifier>(rvm, logisticParameters.first, logisticParameters.second);
		}
		// TODO: Re-think this whole loading logic. As soon as we call loadMatlab here, we lose the ptree, but
		// we want the RvmClassifier to be able to load additional parameters like a "threshold" or "numUsedVectors".
		// Option 1: Make a pair<float sigmA, float sigmB> loadSigmFromML(...); <- changed to this now
		//        2: Here, first call rvm = RvmClassifier::loadConfig(subtree), loads everything.
		//			 Then, here, call prvm = ProbRvmClass::loadMatlab/ProbabilisticStuff(rvm, thresholdsFile);
		else 
		{
			throw logic_error("ProbabilisticRvmClassifier: Only loading of .mat RVMs is supported. If you want to load a non-cascaded RVM, use an SvmClassifier.");
		}
	}

	

	std::pair<double, double> ProbabilisticRvmClassifier::loadSigmoidParamsFromMATLAB(const std::string& logisticFileName)
	{
		Logger logger = LoggerFactory::Instance()->getLogger("classification");


		// Load sigmoid parameters
		double logisticA, logisticB;
		mat_t* pmatfile;
		matvar_t* pmxarray;
		pmatfile = Mat_Open(logisticFileName.c_str(), MAT_ACC_RDONLY);
		if (pmatfile == NULL)
		{
			throw std::invalid_argument("ProbabilisticRvmClassifier: Unable to open the thresholds-file: " + logisticFileName);
		}

		// Check for posterior_wrvm
		pmxarray = Mat_VarRead(pmatfile, "posterior_wrvm");
		if (pmxarray == NULL)
		{
			throw std::runtime_error("ProbabilisticRvmClassifier: Unable to find the vector posterior_wrvm. If you don't want probabilistic output, don't use a probabilistic classifier.");
		}
		else
		{
			double* matdata = (double*)pmxarray->data;
			if (pmxarray->dims[1] != 2)
			{
				throw std::runtime_error("ProbabilisticRvmClassifier: Size of vector posterior_wrvm != 2. If you don't want probabilistic output, don't use a probabilistic classifier.");
			}
			else
			{
				logisticB = matdata[0];
				logisticA = matdata[1];
			}
			Mat_VarFree(pmxarray);
		}
		Mat_Close(pmatfile);
		std::pair<double, double> sigmoidParams(logisticA, logisticB);
		return sigmoidParams;

		return std::make_pair(logisticA, logisticB);

		std::string errorMessage("ProbabilisticRvmClassifier: Cannot load a Matlab classifier, library compiled without support for Matlab. Please re-run CMake with WITH_MATLAB_CLASSIFIER enabled.");
		logger.error(errorMessage);
		throw std::runtime_error(errorMessage);

		

	}
	
}