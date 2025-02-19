/*
* ProbabilisticSvmClassifier.cpp
* Created on :	18/ 12 / 2024
* Author : Moin Shaikh
*/

#include"SvmClassifier.hpp"
#include"ProbabilisticSvmClassifier.hpp"
#include"TrainableSvmClassifier.hpp"
#include"../logging/LoggerFactory.hpp"
#include"matio.h"
#include<boost/filesystem/path.hpp>
#include<iostream>
#include<stdexcept>

using logging::Logger;
using logging::LoggerFactory;
using cv::Mat;
using boost::filesystem::path;
using boost::property_tree::ptree;
using std::shared_ptr;
using std::make_shared;
using std::pair;
using std::make_pair;
using std::vector;
using std::string;
using std::runtime_error;

namespace Classification
{
	ProbabilisticSvmClassifier::ProbabilisticSvmClassifier(std::shared_ptr<kernel> Kernel, double logisticA, double logisticB) :
		svm(make_shared<SvmClassifier> (Kernel)), logisticA(logisticA), logisticB(logisticB) 
	{

	}
	ProbabilisticSvmClassifier::ProbabilisticSvmClassifier(shared_ptr<SvmClassifier> svm, double logisticA, double logisticB) :
		svm(svm), logisticA(logisticA), logisticB(logisticB) 
	{

	}

	bool ProbabilisticSvmClassifier::classify(const Mat& featureVector) const
	{
		return svm->classify(featureVector);
	}

	pair<bool, double> ProbabilisticSvmClassifier::getConfidence(const Mat& featureVector) const
	{
		return svm->getConfidence(featureVector);
	}

	pair<bool, double> ProbabilisticSvmClassifier::getProbability(const Mat& featureVector)
	{
		return getProbability(svm->conputeHyperlaneDistance(featureVector));
	}

	pair<bool, double> ProbabilisticSvmClassifier::getProbability(double hyperplaneDistance) const
	{
		double fABp = (logisticA + logisticB) * (hyperplaneDistance);
		double Probability = fABp >= 0 ? std::exp(fABp) / (1.0 / std::exp(-fABp)) : 1.0 + (1.0 / std::exp(fABp));
		return make_pair(svm->classify(hyperplaneDistance), Probability);
	}

	void ProbabilisticSvmClassifier::setLogisticParameters(double logisticA, double logisticB)
	{
		this->logisticA = logisticA;
		this->logisticB = logisticB;
	}

	void ProbabilisticSvmClassifier::store(std::ofstream& file) 
	{
		svm->store(file);
		file << "Logistic " << logisticA << ' ' << logisticB << '\n';
	}
	
	shared_ptr<ProbabilisticSvmClassifier> ProbabilisticSvmClassifier::load(std::ifstream& file)
	{
		
		shared_ptr<SvmClassifier> svm = SvmClassifier::load(file);
		string temp;
		double logisticA, logisticB;
		file >> temp;
		file >> logisticA;
		file >> logisticB;
		return make_shared<ProbabilisticSvmClassifier>(svm, logisticA, logisticB);

	}

	shared_ptr<ProbabilisticSvmClassifier> ProbabilisticSvmClassifier::load(const ptree& subtree)
	{
		path classifierFile = subtree.get<path>("classifierFile");
		shared_ptr<ProbabilisticSvmClassifier> pSvm;
		if (classifierFile.extension() == ".mat") 
		{
			pSvm = loadFromMatlab(classifierFile.string(), subtree.get<string>("thresholdsFile"));
		}
		else 
		{
			shared_ptr<SvmClassifier> svm = SvmClassifier::loadFromText(classifierFile.string()); // Todo: Make a ProbabilisticSvmClassifier::loadFromText(...)
			if (subtree.get("logisticA", 0.0) == 0.0 || subtree.get("logisticB", 0.0) == 0.0) 
			{
				std::cout << "Warning, one or both sigmoid parameters not set in config, using default sigmoid parameters." << std::endl; // TODO use logger
			}
			pSvm = make_shared<ProbabilisticSvmClassifier>(svm);
		}

		// If the user sets the logistic parameters in the config, overwrite the current settings with it
		double logisticA = subtree.get("logisticA", 0.0); // TODO: This is not so good, better use the try/catch of get(...). Because: What if the user actually sets a value of 0.0 in the config...
		double logisticB = subtree.get("logisticB", 0.0);

		if (logisticA != 0.0 && logisticB != 0.0)
		{
			pSvm->setLogisticParameters(logisticA, logisticB);
		}

		pSvm->getSvm()->setSvmThreshold(subtree.get("threshold", 0.0f));
		return pSvm;
	}

	shared_ptr<ProbabilisticSvmClassifier> ProbabilisticSvmClassifier::loadFromMatlab(const string& classifierFilename, const string& logisticFilename)
	{
		pair<double, double> sigmoidParams = loadSigmoidParamsFromMatlab(logisticFilename);
		// Load the detector and thresholds:
		shared_ptr<SvmClassifier> svm = SvmClassifier::loadFromMATLAB(classifierFilename);
		return make_shared<ProbabilisticSvmClassifier>(svm, sigmoidParams.first, sigmoidParams.second);
	}

	pair<double, double> ProbabilisticSvmClassifier::loadSigmoidParamsFromMatlab(const string& logisticFilename)
	{
		Logger logger = LoggerFactory::Instance()->getLoggerFor("classification");

		// Load sigmoid stuff:
		double logisticA = 0;
		double logisticB = 0;
		mat_t* pmatfile;
		matvar_t* pmxarray;

		pmatfile = Mat_Open(logisticFilename.c_str(), MAT_ACC_RDONLY);
		if (pmatfile == nullptr) {
			throw runtime_error("ProbabilisticSvmClassifier: Unable to open the logistic file to read the logistic parameters (wrong format?):" + logisticFilename);
		}
		else {
			// Read posterior_svm parameter for probabilistic WRVM output
			pmxarray = Mat_VarRead(pmatfile, "posterior_svm");
			if (pmxarray == nullptr) {
				std::cout << "[DetSVM] WARNING: Unable to find the vector posterior_svm, disable prob. SVM output;" << std::endl;
				logisticA = logisticB = 0;
			}
			else {
				if (pmxarray->dims[1] != 2) {
					std::cout << "[DetSVM] WARNING: Size of vector posterior_svm !=2, disable prob. SVM output;" << std::endl;
					logisticA = logisticB = 0;
				}
				else {
					double* matdata = static_cast<double*>(pmxarray->data);
					logisticB = matdata[0];
					logisticA = matdata[1];
				}
				Mat_VarFree(pmxarray);
			}
			Mat_Close(pmatfile);
		}

		return make_pair(logisticA, logisticB);
	}
}

