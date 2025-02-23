#pragma once
/*
* File: IntegralGradientFilter.hpp
* Created on: 10/2/2025
* Author : Moin Shaikh
*/

#ifndef FEATUREDETECTON_IMAGEPROCESSING_INTEGRALGRADIENTFILTER_HPP_
#define FEATUREDETECTON_IMAGEPROCESSING_INTEGRALGRADIENTFILTER_HPP_
 

#include"ImageFilter.hpp"


namespace ImageProcessing
{

	/**
 * Filter that expects an integral image and computes horizontal and vertical gradients by taking the differences of
 * neighboring regions across the image. The input image must be of type CV_32SC1, the output image will be of type
 * CV_8UC2.
 */
	class IntegralGradientFilter : public ImageFilter 
	{
	public:

		/**
		 * Constructs a new integral gradient filter.
		 *
		 * @param[in] rows Row count of points at which to compute the gradients.
		 * @param[in] cols Column count of points at which to compute the gradients.
		 */
		IntegralGradientFilter(int rows, int cols);

		/**
		 * Constructs a new integral gradient filter with square results.
		 *
		 * @param[in] count Row and column count of points at which to compute the gradients.
		 */
		IntegralGradientFilter(int count);

		using ImageFilter::applyTo;

		cv::Mat applyTo(const cv::Mat& image, cv::Mat& filtered) const;

	private:

		int rows; ///< Row count of points at which to compute the gradients.
		int cols; ///< Column count of points at which to compute the gradients.
	};
}

#endif //FEATUREDETECTON_IMAGEPROCESSING_INTEGRALGRADIENTFILTER_HPP_