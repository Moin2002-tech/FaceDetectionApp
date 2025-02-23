#pragma once
/*
* ClassificationBasedStateValidator.hpp
* Created on: 22/1/2025
* Author : Moin Shaikh
*/

#ifndef FEATUREDETECTION_CONDENSATION_CLASSIFICATIONBASEDSTATEVALIDATOR_HPP_
#define FEATUREDETECTION_CONDENSATION_CLASSIFICATIONBASEDSTATEVALIDATOR_HPP_

#include"StateValidator.hpp"
namespace ImageProcessing
{
	class FeatureExtractor;
	class VersionedImage;

}

namespace Classification
{
	class BinaryClassifier;
}


namespace Condensation
{
	/**
	 * State validator that extracts and classifies patches around the target state. At least one of those patches
	 * must be positively classified in order to validate the target state.
	*/


	class ClassificationBasedStateValidator : public StateValidator {
	public:

		/**
		 * Constructs a new classification based state validator.
		 *
		 * @param[in] extractor Feature extractor.
		 * @param[in] classifier Feature classifier.
		 * @param[in] sizes Sizes around the target size that are searched for a valid patch.
		 * @param[in] displacements Displacements of the target position that are searched for a valid patch.
		 */
		ClassificationBasedStateValidator(
			std::shared_ptr<ImageProcessing::FeatureExtractor> extractor, std::shared_ptr<Classification::BinaryClassifier> classifier,
			std::vector<double> sizes = { 1 }, std::vector<double> displacements = { 0 });

		bool isValid(const Sample& target, const std::vector<std::shared_ptr<Sample>>& samples,
			std::shared_ptr<ImageProcessing::VersionedImage> image);

	private:

		std::shared_ptr<ImageProcessing::FeatureExtractor> extractor; ///< Feature extractor.
		std::shared_ptr<Classification::BinaryClassifier> classifier; ///< Feature classifier.
		std::vector<double> sizes; ///< Sizes around the target size that are searched for a valid patch.
		std::vector<double> displacements; ///< Displacements of the target position that are searched for a valid patch.
	};
}

#endif //FEATUREDETECTION_CONDENSATION_CLASSIFICATIONBASEDSTATEVALIDATOR_HPP_