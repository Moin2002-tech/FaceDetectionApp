/*
* TrainableSvmClassifier.hpp
* Created on : 18/ 11 / 2024
* Author : Moin Shaikh
*/
#include"TrainableSvmClassifier.hpp"
#include"SvmClassifier.hpp"
#include"Kernel.hpp"

using cv::Mat;
using std::vector;
using std::pair;
using std::make_pair;
using std::shared_ptr;
using std::make_shared;

namespace Classification
{
	
	TrainableSvmClassifier::TrainableSvmClassifier(shared_ptr<SvmClassifier> svm) :
		svm(svm), usable(false)
	{

	}
	
	TrainableSvmClassifier::TrainableSvmClassifier(shared_ptr<kernel> Kernel) :
		svm(make_shared<SvmClassifier>(Kernel)), usable(false) 
	{

	}



	TrainableSvmClassifier::~TrainableSvmClassifier() {}

	bool TrainableSvmClassifier::classify(const Mat& featureVector) const 
	{
		return svm->classify(featureVector);
	}
	pair<bool, double> TrainableSvmClassifier::getConfidence(const Mat& featureVector) const 
	{
		return svm->getConfidence(featureVector);
	}

	bool TrainableSvmClassifier::isUsable() const
	{
		return usable;
	}

	shared_ptr<SvmClassifier> TrainableSvmClassifier::getSvm() 
	{
		return svm;
	}

	const shared_ptr<SvmClassifier> TrainableSvmClassifier::getSvm() const
	{
		return svm;
	}
}