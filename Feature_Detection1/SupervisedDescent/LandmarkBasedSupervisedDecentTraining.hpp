#pragma once
/*
* File: LandmarkBaseSupervisedDecentTraining.hpp
* Created on: 19/7/2025
* Author : Moin Shaikh
*/


#ifndef FEATUREDETECTION_SUPERVISEDDESCENT_LANDMARKBASESUPERVISEDDECENTTRAINING_HPP
#define FEATUREDETECTION_SUPERVISEDDESCENT_LANDMARKBASESUPERVISEDDECENTTRAINING_HPP

#include"SdmLandMarkModel.hpp"
#include"DiscriptorExtractor.hpp"

#include<opencv2/core.hpp>


namespace superVisedDescent
{
	/**
	 * A class to train a landmark model based on the supervised descent
	 * method of Xiong and De la Torre, "Supervised Descent Method and its
	 * Applications to Face Alignment", CVPR 2013.
	 * The class provides reasonable default arguments, so when calling
	 * the train function without setting any arguments, it works.
	 * However, more detailed stuff can be set by the setters.
	 *
	 * Todo: Write something about how the landmarks are represented?
	*/

	struct GaussParameter
	{
		float mu = 0.0;
		float sigma = 1.0; // Note: sigma = stddev. sigma^2 = variance.
		// Notation is $\mathcal{N}(mu, sigma^2)$ (mean, variance).
		// std::normal_distribution takes (mu, sigma) as arguments.
	};

	// Todo: Note sure what exactly this measures. Think about it.
	struct AlignmentStatistics
	{
		GaussParameter tx; // translation in x-direction
		GaussParameter ty; // ...
		GaussParameter sx;
		GaussParameter sy;
	};

	// Holds the regularisation parameters for the training.
	struct Regularisation
	{
		float factor = 0.5f;
		bool regulariseAffineComponent = false;
		bool regulariseWithEigenvalueThreshold = false;
	};
	
	class LandmarkBaseSupervisedDecentTraining
	{
	
	public:
		/**
		 * Constructs a new LandmarkBasedSupervisedDescentTraining.
		 *
		 * @param[in] a b
		 */
	 //LandmarkBasedSupervisedDescentTraining() {};


		enum class AlignGroundtruth
		{ // what to do with the GT LMs before mean is taken.
			NONE, // no prealign, stay in img-coords
			NORMALIZED_FACEBOX// translate/scale to facebox, that is a normalized square [-0.5, ...] x ...
		};

		enum class MeanNormalization
		{ // what to do with the mean coords after the mean has been calculated
			NONE,
			UNIT_SUM_SQUARED_NORMS // orig paper
		};

		cv::Mat calculateMean(
			cv::Mat& landmark,
			AlignGroundtruth alignGroundTruth,
			MeanNormalization meanNormalization,
			std::vector < cv::Rect > faceBoxes = std::vector<cv::Rect>()
		);

		AlignmentStatistics calculateAligmentStatistics(
			std::vector<cv::Rect>&trainingFaceboxes, 
			cv::Mat& groundtruthLandmarks,
			 cv::Mat &initialShapeEstimateX0
		);

		// Rescale the model-mean by neutralizing it with the current statistics (i.e. dividing by the scale, subtracting transl.) (only necessary if our mean is not normalized to V&J face-box directly in first steps)
		// modifies the input mean!
		cv::Mat rescaleModel(
			cv::Mat& modelMean, 
			const AlignmentStatistics& alignmentStatistics
		);

		// TODO: Move to MatHelpers::duplicateRows(...)
		cv::Mat duplicateGroundtruthShapes(Mat groundtruthLandmarks, int numSamplesPerImage) 
		{
			Mat groundtruthShapes = Mat::zeros((numSamplesPerImage + 1) * groundtruthLandmarks.rows, groundtruthLandmarks.cols, CV_32FC1); // 10 samples + the original data = 11
			
			for (int currImg = 0; currImg < groundtruthLandmarks.rows; ++currImg) 
			{
				Mat groundtruthLandmarksRow = groundtruthLandmarks.row(currImg);

				for (int j = 0; j < numSamplesPerImage + 1; ++j)
				{
					Mat groundtruthShapesRow = groundtruthShapes.row(currImg * (numSamplesPerImage + 1) + j);
					groundtruthLandmarksRow.copyTo(groundtruthShapesRow);
				}
			}
			return groundtruthShapes;
		};

		void setNumSamplesPerImage(int numSamplesPerImage) 
		{
			this->numSamplesPerImage = numSamplesPerImage;
		};

		void setNumCascadeSteps(int numCascadeSteps)
		{
			this->numCascadeSteps = numCascadeSteps;
		};

		void setRegularisation(Regularisation regularisation)
		{
			this->regularisation = regularisation;
		};

		void setAlignGroundtruth(AlignGroundtruth alignGroundtruth) 
		{
			this->alignGroundtruth = alignGroundtruth;
		};

		void setMeanNormalization(MeanNormalization meanNormalization) 
		{
			this->meanNormalization = meanNormalization;
		};
	public:
			SdmLandmarkModel train(
				std::vector<cv::Mat>& trainingImages,
				std::vector<cv::Mat>& trainingGroundtruthLandmarks,
				std::vector<cv::Rect>& trainingFaceboxes /*maybe optional bzw weglassen hier?*/,
				std::vector<std::string>& modelLandmarks,
				std::vector<std::string>& descriptorTypes,
				std::vector<std::shared_ptr<DescriptorExtractor>>& descriptorExtractors
			);
	private:
		int numSamplesPerImage = 10; ///< How many random perturbations to generate per training image
		int numCascadeSteps = 5; ///< How many regressors to train

		Regularisation regularisation; ///< Controls the regularisation of the regressor learning
		AlignGroundtruth alignGroundtruth = AlignGroundtruth::NONE; ///< For mean calc: todo
		MeanNormalization meanNormalization = MeanNormalization::UNIT_SUM_SQUARED_NORMS; ///< F...Mean: todo
		// Transforms one row...
		// Takes the face-box as [-0.5, 0.5] x [-0.5, 0.5] and transforms the landmarks into that rectangle.
		// lms are x1 x2 .. y1 y2 .. row-vec
		cv::Mat transformLandmarksNormalized(cv::Mat landmarks, cv::Rect box);

		// assumes modelMean is row-vec, first half x, second y.
		cv::Mat meanNormalizationUnitSumSquaredNorms(cv::Mat &modelMean);
	
		
	};

	

}

#endif
