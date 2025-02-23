#pragma once
/*
* File: SelfLearningMeasuremenModel.hpp
* Created on: 2/2/2025
* Author : Moin Shaikh
*/


#ifndef FEATUREDETECTION_CONDENSATION_SELFLEARNINGMEASUREMENTMODEL_HPP_
#define FEATUREDETECTION_CONDENSATION_SELFLEARNINGMEASUREMENTMODEL_HPP_


#include"AdaptiveMeasurementModel.hpp"
#include<opencv2/core.hpp>
#include<string>
#include<unordered_map>
#include<utility>

using std::string;
using cv::Mat;

namespace ImageProcessing
{
	class Patch;
	class FeatureExtractor;
}

namespace Classification
{
	class TrainableProbabilisticClassifier;
}

namespace detection
{
	class ClassifiedPatch;
}


namespace Condensation
{
	class SelfLearningMeasurementModel : public AdaptiveMeasurementModel
	{
	public:

		/**
		 * Constructs a new self-learning measurement model.
		 *
		 * @param[in] featureExtractor The feature extractor used with the dynamic SVM.
		 * @param[in] classifier The classifier that will be re-trained.
		 * @param[in] positiveThreshold The certainty threshold for patches to be used as positive samples (must be exceeded).
		 * @param[in] negativeThreshold The certainty threshold for patches to be used as negative samples (must fall below).
		*/

		SelfLearningMeasurementModel(std::shared_ptr<ImageProcessing::FeatureExtractor> featureExtractor,
			std::shared_ptr<Classification::TrainableProbabilisticClassifier> classifier,
			double positiveThreshold = 0.85, double negativeThreshold = 0.05);

		void update(std::shared_ptr<ImageProcessing::VersionedImage> image);

		void evaluate(Sample& sample) const;

		using AdaptiveMeasurementModel::evaluate;

		bool isUsable() const 
		{
			return usable;
		}

		bool initialize(std::shared_ptr<ImageProcessing::VersionedImage> image, Sample& target);

		bool adapt(std::shared_ptr<ImageProcessing::VersionedImage> image, const std::vector<std::shared_ptr<Sample>>& samples, const Sample& target);

		bool adapt(std::shared_ptr<ImageProcessing::VersionedImage> image, const std::vector<std::shared_ptr<Sample>>& samples);

		void reset();

		/**
		 * Creates a list of feature vectors from the given classified patches.
		 *
		 * @param[in] pairs The extracted and classified patches.
		 * @return The feature vectors.
		*/
		std::vector<cv::Mat> getFeatureVectors(std::vector<std::shared_ptr<detection::ClassifiedPatch>>& patches);

		/**
		 * Creates a list of feature vectors from the given samples.
		 *
		 * @param[in] samples The samples.
		 * @return The extracted feature vectors.
		 */
		std::vector<cv::Mat> getFeatureVectors(std::vector<std::shared_ptr<Sample>>& samples);
	private:



		std::shared_ptr<ImageProcessing::FeatureExtractor> featureExtractor; ///< The feature extractor used with the dynamic SVM.
		std::shared_ptr<Classification::TrainableProbabilisticClassifier> classifier; ///< The classifier that will be re-trained.
		bool usable; ///< Flag that indicates whether this model may be used for evaluation.
		mutable std::unordered_map<std::shared_ptr<ImageProcessing::Patch>, std::pair<bool, double>> cache; ///< The classification result cache.
		double positiveThreshold; ///< The threshold for samples to be used as positive training samples (must be exceeded).
		double negativeThreshold; ///< The threshold for samples to be used as negative training samples (must fall below).
		mutable std::vector<std::shared_ptr<detection::ClassifiedPatch>> positiveTrainingExamples; ///< The positive training examples.
		mutable std::vector<std::shared_ptr<detection::ClassifiedPatch>> negativeTrainingExamples; ///< The negative training examples.
	};
}
#endif //FEATUREDETECTION_CONDENSATION_SELFLEARNINGMEASUREMENTMODEL_HPP_