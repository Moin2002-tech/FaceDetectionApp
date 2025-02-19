
/*
* ProbabilisticTwoStageClassifier.cpp
* Created on :	18/ 12 / 2024
* Author : Moin Shaikh
*/

#include"ProbabilisticTwoStageClassifier.hpp"

using cv::Mat;
using std::pair;
using std::shared_ptr;

namespace Classification
{
	ProbabilisticTwoStagesClassifier::ProbabilisticTwoStagesClassifier(
		shared_ptr<ProbabilisticClassifier> first, shared_ptr<ProbabilisticClassifier> second) :
		first(first), second(second) {
	}

	ProbabilisticTwoStagesClassifier::~ProbabilisticTwoStagesClassifier() {}

	bool ProbabilisticTwoStagesClassifier::classify(const Mat& featureVector) const {
		if (first->classify(featureVector))
			return second->classify(featureVector);
		return false;
	}

	pair<bool, double> ProbabilisticTwoStagesClassifier::getConfidence(const Mat& featureVector) const 
	{
		pair<bool, double> result = first->getConfidence(featureVector);
		if (result.first)
			return second->getConfidence(featureVector);
		return result;
	}

	pair<bool, double> ProbabilisticTwoStagesClassifier::getProbability(const Mat& featureVector) 
	{
		pair<bool, double> result = first->getProbability(featureVector);
		if (result.first)
			return second->getConfidence(featureVector);
		return result;
	}
}