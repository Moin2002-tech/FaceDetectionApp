#pragma once
/*
* File: LinearRegression.hpp
* Created on: 19/7/2025
* Author : Moin Shaikh
*/


#ifndef FEATUREDETECTION_SUPERVISEDDESCENT_LINEARREGRESSION_HPP_
#define FEATUREDETECTION_SUPERVISEDDESCENT_LINEARREGRESSION_HPP_

#include"LandmarkBasedSupervisedDecentTraining.hpp"

#include<opencv2/core.hpp>


namespace superVisedDescent
{
	enum class RegularizationType
	{
		Manual, ///< use lambda
		Automatic, ///< use norm... optional lambda used as factor
		EigenvalueThreshold ///< see description libEigen
	};
	class LinearRegression
	{
	public:
		LinearRegression() { }

		cv::Mat alignMean(
			cv::Mat& mean,
			cv::Rect& face,
			float scalingX = 1.0f,
			float scalingY = 1.0f,
			float translationX = 0.0f,
			float translationY = 0.0f
		);


		float calculateMeanTranslation(cv::Mat& groundTruth, cv::Mat& estimate);

		float calculateScaleRatio(cv::Mat& groundTruth, cv::Mat& estimate);

		void saveShapeInstanceToMatlab(cv::Mat& shapeInstance, std::string& filename);

		void DrawLandmark(cv::Mat& image, cv::Mat &landmark, cv::Scalar color = cv::Scalar(0, 255, 0));

		cv::Mat getPerturbedShape(
			cv::Mat modelMean,
			AlignmentStatistics alignmentStatistics,
			cv::Rect detectedFace
		);



		/**
		 * Todo.
		 *
		 * @param[in] A The todo.
		 * @param[in] b The todo.
		 * @param[in] regularizationType Todo.
		 * @param[in] lambda For RegularizationType::Automatic: An optional factor to multiply the automatically calculated value with.
		 *                   For RegularizationType::Manual: The absolute value of the regularization term.
		 * @param[in] regularizeAffineComponent Flag that indicates whether to regularize the affine component as well (the last column(?) of AtA (?)). Default: true
		 * @return x.
		*/
		cv::Mat linearRegression(
			cv::Mat &A,
			cv::Mat &b,
			RegularizationType regularizationType = RegularizationType::Automatic,
			float lambda = 0.5f, 
			bool regularizeAffineComponent = true
		);

		/**
		* Todo.
		*
		* @param[in] matrix The todo.
		* @return todo.
		*/
		float calculateEigenvalueThreshold(cv::Mat &matrix);
	};
}

#endif