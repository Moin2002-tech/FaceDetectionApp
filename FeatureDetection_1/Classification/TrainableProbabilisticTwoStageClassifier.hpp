#pragma once
/*
* TrainableProbabilisticTwoStageClassifier.hpp
* Created on : 22/ 11 / 2024
* Author : Moin Shaikh
*/

#ifndef FEATUREDETECTION_CLASSIFICATION_TRAINABLEPROBABILISTICTWOSTAGECLASSIFIER_HPP_
#define FEATUREDETECTION_CLASSIFICATION_TRAINABLEPROBABILISTICTWOSTAGECLASSIFIER_HPP_

#include"ProbabilisticTwoStageClassifier.hpp"
#include"TrainableProbabilisticClassifier.hpp"

namespace Classification 
{
	/**
	 * Probabilistic two-stage classifier whose second classifier can be re-trained using new examples.
	*/
	class TrainableProbabilisticTwoStageClassifier : public ProbabilisticTwoStagesClassifier, public TrainableProbabilisticClassifier
	{
	public:
		/**
		 * Constructs a new trainable probabilistic two-stage classifier.
		 *
		 * @param[in] first The first classifier.
		 * @param[in] second The second classifier (that will be re-trained).
		*/
		TrainableProbabilisticTwoStageClassifier(std::shared_ptr<ProbabilisticClassifier> first, std::shared_ptr<TrainableProbabilisticClassifier> second) :
			ProbabilisticTwoStagesClassifier(first, second), trainable(second) {}

		~TrainableProbabilisticTwoStageClassifier() {}

		bool classify(const cv::Mat& featureVector) const {
			return ProbabilisticTwoStagesClassifier::classify(featureVector);
		}

		std::pair<bool, double> getConfidence(const cv::Mat& featureVector) const {
			return ProbabilisticTwoStagesClassifier::getConfidence(featureVector);
		}

		std::pair<bool, double> getProbability(const cv::Mat& featureVector) const {
			return ProbabilisticTwoStagesClassifier::getProbability(featureVector);
		}

		bool isUsable() const {
			return trainable->isUsable();
		}

		bool retrain(const std::vector<cv::Mat>& newPositiveExamples, const std::vector<cv::Mat>& newNegativeExamples) {
			return trainable->retrain(newPositiveExamples, newNegativeExamples);
		}

		bool retrain(const std::vector<cv::Mat>& newPositiveExamples, const std::vector<cv::Mat>& newNegativeExamples,
			const std::vector<cv::Mat>& newPositiveTestExamples, const std::vector<cv::Mat>& newNegativeTestExamples) {
			return trainable->retrain(newPositiveExamples, newNegativeExamples, newPositiveTestExamples, newNegativeTestExamples);
		}

		void reset() {
			return trainable->reset();
		}

		
	private:
		std::shared_ptr<TrainableProbabilisticClassifier> trainable;
		
	
	};
}

#endif //FEATUREDETECTION_CLASSIFICATION_TRAINABLEPROBABILISTICTWOSTAGECLASSIFIER_HPP_