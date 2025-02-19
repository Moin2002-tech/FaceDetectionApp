#pragma once
/*
* ProbabilisticTwoStagesClassifier.hpp
* Created on : 18/ 11 / 2024
* Author : Moin Shaikh
*/
#ifndef FEATUREDETECTION_CLASSIFICATION_PROBABILISTIC_TWOSTAGESCLASSIFIER_HPP_ 
#define FEATUREDETECTION_CLASSIFICATION_PROBABILISTIC_TWOSTAGESCLASSIFIER_HPP_
#include"ProbabilisticClassifier.hpp"
#include<memory>

namespace Classification
{

	class ProbabilisticTwoStagesClassifier : public ProbabilisticClassifier
	{
	public:
		/**
		 * Constructs a new probabilistic two-stage classifier.
		 *
		 * @param[in] first The first classifier.
		 * @param[in] second The second classifier.
		*/
		explicit ProbabilisticTwoStagesClassifier(std::shared_ptr<ProbabilisticClassifier> first, std::shared_ptr<ProbabilisticClassifier> second);
		virtual ~ProbabilisticTwoStagesClassifier();
		bool classify(const cv::Mat& featureVector) const;
		std::pair<bool, double> getConfidence(const cv::Mat& featureVector) const;
		std::pair<bool, double> getProbability(const cv::Mat& feataureVector);
	private:
		std::shared_ptr<ProbabilisticClassifier> first;  ///< The first classifier.
		std::shared_ptr<ProbabilisticClassifier> second; ///< The second classifier.
	
	};
}

#endif // 
