#pragma once
/*
* TrainableProbabilisticClassifier.hpp
* Created on : 19 / 10 / 2024
* Author : Moin Shaikh
*/

#ifndef FEATUREDETECTION_CLASSIFICATION_TRAINABLE_PROBABILISTIC_CLASSIFIER_HPP_
#define FEATUREDETECTION_CLASSIFICATION_TRAINABLE_PROBABILISTIC_CLASSIFIER_HPP_

#include"TrainableClassifier.hpp"
#include"ProbabilisticClassifier.hpp"

namespace Classification
{
	class TrainableProbabilisticClassifier : public TrainableClassifier, public ProbabilisticClassifier
	{
	public:
		virtual ~TrainableProbabilisticClassifier() {};
		/**
		 * Re-trains this classifier incrementally, adding new training examples. The training examples will also
		 * be used as test examples to determine the parameters of the logistic function for probabilistic output.
		 * May not change the classifier if there is not enough training data.
		 *
		 * @param[in] newPositiveExamples The new positive training and test examples.
		 * @param[in] newNegativeExamples The new negative training and test examples.
		 * @return True if this classifier was trained successfully and may be used, false otherwise.
		*/
		virtual bool retrain(const std::vector<cv::Mat>& newPositiveExample, const std::vector<cv::Mat>& newNegativeExample) = 0;

		/**
		 * Re-trains this classifier incrementally, adding new training examples. The test examples will be used to
		 * determine the parameters of the logistic function for probabilistic output. May not change the classifier
		 * if there is not enough training data.
		 *
		 * @param[in] newPositiveExamples The new positive training examples.
		 * @param[in] newNegativeExamples The new negative training examples.
		 * @param[in] newPositiveTestExamples The new positive test examples.
		 * @param[in] newNegativeTestExamples The new negative test examples.
		 * @return True if this classifier was trained successfully and may be used, false otherwise.
	    */
		virtual bool retrain(const std::vector<cv::Mat>& newPositiveExamples, const std::vector<cv::Mat>& newNegativeExamples,
			const std::vector<cv::Mat>& newPositiveTestExamples, const std::vector<cv::Mat>& newNegativeTestExamples) = 0;
	};
} // namespace Classification

#endif //FEATUREDETECTION_CLASSIFICATION_TRAINABLE-PROBABILISTIC-CLASSIFIER_HPP_