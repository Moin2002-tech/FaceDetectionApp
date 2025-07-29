#pragma once
/*
* TrainableSvmClassifier.hpp
* Created on :20/ 11 / 2024
* Author : Moin Shaikh
*/
#ifndef FEATUREDETECTION_CLASSIFICATION_TRAINABLESVMCLASSIFIER_HPP_
#define FEATUREDETECTION_CLASSIFICATION_TRAINABLESVMCLASSIFIER_HPP_

#include"TrainableBinaryClassifier.hpp"
#include<memory>
#include<utility>

namespace Classification
{
	class SvmClassifier;
	class kernel;

	class TrainableSvmClassifier : public TrainableBinaryClassifier
	{
	public:

		/**
		 * Constructs a new trainable SVM classifier that wraps around the actual SVM classifier.
		 *
		 * @param[in] svm The actual SVM.
		*/
		explicit TrainableSvmClassifier(std::shared_ptr<SvmClassifier> svm);
		/**
		 * Constructs a new trainable SVM classifier.
	     *
		 * @param[in] kernel The kernel function.
		*/
		explicit TrainableSvmClassifier(std::shared_ptr<kernel> Kernel);
		
		virtual ~TrainableSvmClassifier();

		bool classify(const cv::Mat& featureVector) const;

		std::pair<bool, double> getConfidence(const cv::Mat& featureVector) const;

		bool isUsable() const;
		/**
		 * @return The actual SVM.
		*/
		std::shared_ptr<SvmClassifier> getSvm();

		/**
		* @return The actual SVM.
		*/
		const std::shared_ptr<SvmClassifier> getSvm() const;

	protected:
		std::shared_ptr<SvmClassifier> svm;
		bool usable;


	};
}

#endif //FEATUREDETECTION_CLASSIFICATION_TRAINABLESVMCLASSIFIER_HPP_