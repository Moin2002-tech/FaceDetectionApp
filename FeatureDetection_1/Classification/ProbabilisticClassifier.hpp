#pragma once
/* 
* ProbabilisticClassifier.hpp
* Created on : 19 / 10 / 2024
* Author : Moin Shaikh
*/
#ifndef FEATUREDETECTION_CLASSIFICATION_PROBABILISTICCLASSIFIER_HPP_
#define FEATUREDETECTION_CLASSIFICATION_PROBABILISTICCLASSIFIER_HPP_
#include"BinaryClassifier.hpp"
#include<memory>
namespace Classification
{
	class ProbabilisticClassifier : public BinaryClassifier
	{
	public:
	
		virtual ~ProbabilisticClassifier() {}
	
		/**
		 * Computes the probability of a feature vector belonging to the positive class.
		 *
		 * @param[in] featureVector The feature vector.
		 * @return A pair containing the binary classification result and a probability between zero and one for being positive.
		*/
		virtual std::pair<bool, double> getProbability(const cv::Mat& featureVector)= 0;


		

	};
} //namespace Classification
#endif //FEATUREDETECTION_CLASSIFICATION_PROBABILISTICCLASSIFIER_HPP_