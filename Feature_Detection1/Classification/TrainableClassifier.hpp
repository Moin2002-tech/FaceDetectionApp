#pragma once
/*
* TrainableClassifier.hpp
* Created on: 19 /10/ 2024
* Author : Moin Shaikh
*/


#ifndef FEATUREDETECTION_CLASSIFICATION_TRAINABLECLASSIFIER_HPP_
#define FEATUREDETECTION_CLASSIFICATION_TRAINABLECLASSIFIER_HPP_

#include "opencv2/core/core.hpp"
#include <vector>
 

namespace Classification
{	
	class TrainableClassifier
	{
	public:
		virtual ~TrainableClassifier(){}
		/**
		* Determines whether this classifier was trained successfully and may be used.
		*	
		* @return True if this classifier was trained successfully and may be used, false otherwise.
		*/
		virtual bool isUsable() const = 0;
		/**
		 * Re-trains this classifier incrementally, adding new training examples. May not change the classifier
		 * if there is not enough training data.
		 *
		 * @param[in] newPositiveExamples The new positive training examples.
		 * @param[in] newNegativeExamples The new negative training examples.
		 * @return True if this classifier was trained successfully and may be used, false otherwise.
		*/
		virtual bool retrain(const std::vector<cv::Mat>& newPositiveExamples, const std::vector<cv::Mat>& newNegativeExamples) = 0;
		/**
		* Resets this classifier.
		*/
		virtual void reset() = 0;

	};
}
#endif //FEATUREDETECTION_CLASSIFICATION_TRAINABLECLASSIFIER_HPP_