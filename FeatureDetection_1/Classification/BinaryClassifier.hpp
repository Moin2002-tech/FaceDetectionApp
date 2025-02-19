#pragma once
/*
* BinaryClassifier.hpp
* Created on : 19 / 10 / 2024
* Author : Moin Shaikh
*/

#ifndef FEATUREDETECTION_CLASSIFICATION_BINARYCLASSIFIER_HPP_
#define FEATUREDETECTION_CLASSIFICATION_BINARYCLASSIFIER_HPP_
#include<opencv2/core.hpp>
#include<utility>

namespace Classification 
{
	/**
	* Classifier that determines whether a feature vector is of a certain class or not.
	*/
	class BinaryClassifier
	{
	public:
		virtual ~BinaryClassifier(){}
		/**
		 * Determines whether a feature vector belongs to the positive class.
		 *
		 * @param[in] featureVector The feature vector.
		 * @return True if the feature vector was positively classified, false otherwise.
		*/
		virtual bool classify(const cv::Mat& featureVector) const = 0;
		/**
		 * Computes the classification confidence of a feature vector. The confidence value will
		 * be high for confident classifications and low for not so confident ones, independent
		 * of the classification result (whether positive or negative).
		 *
		 * @param[in] featureVector The feature vector.
		 * @return A pair containing the binary classification result and the confidence of the classification.
		*/
		virtual std::pair<bool, double> getConfidence(const cv::Mat& featureVector) const = 0;

	};

} //Classification 

#endif //FEATUREDETECTION_CLASSIFICATION_BINARYCLASSIFIER_HPP_