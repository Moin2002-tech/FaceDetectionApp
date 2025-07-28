#pragma once


/*
* File: WvmSvmModel.hpp
* Created on: 4/2/2025
* Author : Moin Shaikh
*/


#ifndef FEATUREDETECTION_CONDENSATION_WVMSVMMODEL_HPP_
#define FEATUREDETECTION_CONDENSATION_WVMSVMMODEL_HPP_


#include"MeasurementModel.hpp"

#include<memory>
#include<unordered_map>
#include<utility>
#include<string>

using std::string;
using std::shared_ptr;
using std::vector;
using std::unordered_map;
using std::make_pair;
using std::make_shared;


namespace ImageProcessing 
{
	class FeatureExtractor;
	class Patch;
}

namespace Classification
{
	class ProbabilisticSvmClassifier;
	class ProbabilisticWvmClassifier;
}
namespace Condensation
{


	/**
	 * Measurement model that uses a WVM for quick elimination and evaluates the samples that remain after an
	 * overlap elimination with a SVM. The weight of the samples will be the product of the certainties from
	 * the two detectors, they will be regarded as being independent (although they are not). The certainties
	 * for the SVM of samples that are not evaluated by it will be chosen to be 0.5 (unknown).
	*/

	class WvmSvmModel : public MeasurementModel
	{
	public:

		/**
		 * Constructs a new WVM-SVM measurement model.
		 *
		 * @param[in] featureExtractor The feature extractor.
		 * @param[in] wvm The fast WVM.
		 * @param[in] svm The slower SVM.
		 * TODO overlap elimination?
		*/
		WvmSvmModel(std::shared_ptr<ImageProcessing::FeatureExtractor> featureExtractor,
			std::shared_ptr<Classification::ProbabilisticWvmClassifier> wvm, std::shared_ptr<Classification::ProbabilisticSvmClassifier> svm);

		void update(std::shared_ptr<ImageProcessing::VersionedImage> image);

		void evaluate(Sample& sample) const;

		void evaluate(std::shared_ptr<ImageProcessing::VersionedImage> image, std::vector<std::shared_ptr<Sample>>& samples);

	private:
		std::shared_ptr<ImageProcessing::FeatureExtractor> featureExtractor; ///< The feature extractor.
		std::shared_ptr<Classification::ProbabilisticWvmClassifier> wvm; ///< The fast WVM.
		std::shared_ptr<Classification::ProbabilisticSvmClassifier> svm; ///< The slower SVM.
		//std::shared_ptr<imageprocessing::OverlapElimination> oe; ///< The overlap elimination algorithm. TODO
		mutable std::unordered_map<std::shared_ptr<ImageProcessing::Patch>, std::pair<bool, double>> cache; ///< The cache of the WVM classification results.
	};
}


#endif //FEATUREDETECTION_CONDENSATION_WVMSVMMODEL_HPP_