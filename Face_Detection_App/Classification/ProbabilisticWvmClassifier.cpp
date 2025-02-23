/*
* ProbabilisticWvmClassifier.cpp
* Created on : 20/ 12 / 2024
* Author : Moin Shaikh
*/


#include"ProbabilisticWvmClassifier.hpp"
#include"WvmClassifier.hpp"
#include"../logging/LoggerFactory.hpp"
#include"../logging/Logger.hpp"
#include<matio.h>
#include<iostream>
#include<stdexcept>


using logging::Logger;
using logging::LoggerFactory;
using cv::Mat;
using boost::property_tree::ptree;
using std::pair;
using std::string;
using std::make_pair;
using std::shared_ptr;
using std::make_shared;
using std::invalid_argument;
using std::runtime_error;


namespace Classification
{

	ProbabilisticWvmClassifier::ProbabilisticWvmClassifier(shared_ptr<WvmClassifier> wvm, double logisticA, double logisticB) :
		wvm(wvm), logisticA(logisticA), logisticB(logisticB) 
	{

	}

	bool ProbabilisticWvmClassifier::classify(const Mat& featureVector) const {
		return wvm->classify(featureVector);
	}

	pair<bool, double> ProbabilisticWvmClassifier::getConfidence(const Mat& featureVector) const
	{
		return wvm->getConfidence(featureVector);
	}

	pair<bool, double> ProbabilisticWvmClassifier::getProbability(const Mat& featureVector) const
	{
		return getProbability(wvm->hyperPlaneDistance(featureVector));
	}


	std::pair<bool, double> ProbabilisticWvmClassifier::getProbability(std::pair<int, double>levelAndDistance) const
	{
		double probability = 1.0f /(1.0f+ std::exp(logisticA + logisticB * levelAndDistance.second));
		return make_pair(wvm->classify(levelAndDistance), probability);
		/*
	// Do sigmoid stuff:
	// NOTE Patrik: Here we calculate the probability for all WVM patches, also of those that
	//      did not run up to the last filter. Those probabilities are wrong, but for the face-
	//      detector it seems to work out ok. However, for the eye-detector, the probability-maps
	//      don't look good. See below for the code.


		 TODO: In case of the WVM, we could again distinguish if we calculate the ("wrong") probability
		 *			of all patches or only the ones that pass the last stage (correct probability), and
		 *			set all others to zero.
		//fp->certainty = 1.0f / (1.0f + exp(posterior_wrvm[0]*fout + posterior_wrvm[1]));
		// We ran till the REAL LAST filter (not just the numUsedFilters one):
		
		if(filter_level+1 == this->numLinFilters && fout >= this->hierarchicalThresholds[filter_level]) 
		{
			certainty = 1.0f / (1.0f + exp(posterior_wrvm[0]*fout + posterior_wrvm[1]));
			return true;
		}
		// We didn't run up to the last filter (or only up to the numUsedFilters one)
		if(this->calculateProbabilityOfAllPatches==true) {
			certainty = 1.0f / (1.0f + exp(posterior_wrvm[0]*fout + posterior_wrvm[1]));
			return false;
		} else {
			certainty = 0.0f;
			return false;
		}
		*/

	}

	shared_ptr<ProbabilisticWvmClassifier> ProbabilisticWvmClassifier::load(const ptree& subtree)
	{
		pair<double, double> sigmoidParams = loadSigmoidParamfromMATLAB(subtree.get<string>("thresholdsFile"));
		// Load the detector and thresholds:
		shared_ptr<WvmClassifier> wvm = WvmClassifier::loadFromMATLAB(subtree.get<string>("classifierFile"), subtree.get<string>("thresholdsFile"));
		shared_ptr<ProbabilisticWvmClassifier> pwvm = make_shared<ProbabilisticWvmClassifier>(wvm, sigmoidParams.first, sigmoidParams.second);

		pwvm->getWvm()->setLimitReliabilityFilter(subtree.get("threshold", 0.0f));

		return pwvm;
	}

	shared_ptr<ProbabilisticWvmClassifier> ProbabilisticWvmClassifier::loadFromMATLAB(const string& classifierFilename, const string& thresholdsFilename)
	{
		pair<double, double> sigmoidParams = loadSigmoidParamfromMATLAB(thresholdsFilename);
		// Load the detector and thresholds:
		shared_ptr<WvmClassifier> wvm = WvmClassifier::loadFromMATLAB(classifierFilename, thresholdsFilename);
		return make_shared<ProbabilisticWvmClassifier>(wvm, sigmoidParams.first, sigmoidParams.second);
	}

	pair<double, double> ProbabilisticWvmClassifier::loadSigmoidParamfromMATLAB(const string& thresholdsFilename)
	{
		Logger logger = LoggerFactory::Instance()->getLogger("Classification");

		// Load sigmoid stuff:
		double logisticA, logisticB;
		mat_t* pmatfile;  // Use mat_t for MAT file
		matvar_t* pmxarray;  // Use matvar_t for MAT variables

		pmatfile = Mat_Open(thresholdsFilename.c_str(), MAT_ACC_RDONLY);
		if (pmatfile == NULL) {
			throw invalid_argument("ProbabilisticWvmClassifier: Unable to open the thresholds-file: " + thresholdsFilename);
		}

		// Load the variable "posterior_wrvm"
		pmxarray = Mat_VarRead(pmatfile, "posterior_wrvm");
		if (pmxarray == NULL) {
			Mat_Close(pmatfile);
			throw runtime_error("ProbabilisticWvmClassifier: Unable to find the variable posterior_wrvm. If you don't want probabilistic output, don't use a probabilistic classifier.");
		}

		// Check if the variable is a 2-element vector
		if (pmxarray->rank != 2 || pmxarray->dims[1] != 2) {
			Mat_VarFree(pmxarray);
			Mat_Close(pmatfile);
			throw runtime_error("ProbabilisticWvmClassifier: Size of variable posterior_wrvm != 2. If you don't want probabilistic output, don't use a probabilistic classifier.");
		}

		// Extract data from the variable
		double* matdata = static_cast<double*>(pmxarray->data);
		logisticB = matdata[0];
		logisticA = matdata[1];

		// Cleanup
		Mat_VarFree(pmxarray);
		Mat_Close(pmatfile);

		return make_pair(logisticA, logisticB);

		string errorMessage("ProbabilisticWvmClassifier: Cannot load a MAT classifier, library compiled without support for MAT. Please re-run CMake with WITH_MATLAB_CLASSIFIER enabled.");
		logger.error(errorMessage);
		throw std::runtime_error(errorMessage);

	}
	
}