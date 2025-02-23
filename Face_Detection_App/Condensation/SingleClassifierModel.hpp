#pragma once
/*
* File: SimpleClassifierModel.hpp
* Created on: 4/2/2025
* Author : Moin Shaikh
*/


#ifndef FEATUREDETECTION_CONDENSATION_SINGLECLASSIFIERMODEL_HPP_
#define FEATUREDETECTION_CONDENSATION_SINGLECLASSIFIERMODEL_HPP_



#include"MeasurementModel.hpp"
#include<unordered_map>
#include<utility>

namespace ImageProcessing
{
	class FeatureExtractor;
	class Patch;
}

namespace Classification
{
	class ProbabilisticClassifier;
}

namespace Condensation
{
	/*
	* Simple measurement model that evaluates each sample by classifying its extracted feature vector.
	*/

	class SingleClassifierModel : public MeasurementModel
	{
	public:
		/**
			* Constructs a new single classifier measurement model.
			*
			* @param[in] featureExtractor The feature extractor.
			* @param[in] classifier The classifier.
		*/
		SingleClassifierModel(std::shared_ptr<ImageProcessing::FeatureExtractor> featureExtractor,
			std::shared_ptr<Classification::ProbabilisticClassifier> classifier);

		void update(std::shared_ptr<ImageProcessing::VersionedImage> image);

		void evaluate(Sample& sample) const;

		using MeasurementModel::evaluate;

	public:
		/**
		 * Classifies a patch using its data.
		 *
		 * @param[in] patch The patch.
		 * @return The classification result.
	   */
		std::pair<bool, double> classify(std::shared_ptr<ImageProcessing::Patch> patch) const;

		std::shared_ptr<ImageProcessing::FeatureExtractor> featureExtractor; ///< The feature extractor.
		std::shared_ptr<Classification::ProbabilisticClassifier> classifier; ///< The classifier.
		mutable std::unordered_map<std::shared_ptr<ImageProcessing::Patch>, std::pair<bool, double>> cache; ///< The classification result cache.
	};
}
#endif //FEATUREDETECTION_CONDENSATION_SINGLECLASSIFIERMODEL_HPP_
