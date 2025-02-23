#pragma once
/*
* File: GradientMagnitudeFilter.hpp
* Created on: 10/2/2025
* Author : Moin Shaikh
*/

#ifndef FEATUREDETECTION_IMAGEPROCESSING_GRADIENTMAGNITUDEFILTER_HPP_
#define FEATUREDETECTION_IMAGEPROCESSING_GRADIENTMAGNITUDEFILTER_HPP_

#include"ImageFilter.hpp"

namespace ImageProcessing
{
	/**
	 * Image filter that expects a gradient image with two channels - each channel corresponds the the gradient of a direction.
	 * The resulting image will have a single channel containing the magnitude of the gradient.
	*/
	class GradientMagnitudeFilter : public ImageFilter
	{
	public:

		GradientMagnitudeFilter();

		using ImageFilter::applyTo;

		cv::Mat applyTo(const cv::Mat& image, cv::Mat& filtered) const;

	private:

		/**
		 * Computes the gradient magnitude of each pixel of an image.
		 *
		 * @param[in] image The image containing the gradients of x and y.
		 * @param[out] filtered The image to store the computed magnitude values into.
		 * @param[in] rows The amount of rows.
		 * @param[in] cols The amount of columns.
		 * @param[in] delta The offset of the gradients (in case the image has an unsigned data type).
		 */
		template<class T>
		void computeMagnitude(const cv::Mat& image, cv::Mat& filtered, int rows, int cols, double delta = 0) const
		{
			for (int row = 0; row < rows; ++row) {
				const cv::Vec<T, 2>* gradientRow = image.ptr<cv::Vec<T, 2>>(row);
				T* magnitudeRow = filtered.ptr<T>(row);
				for (int col = 0; col < cols; ++col) {
					double x = static_cast<double>(gradientRow[col][0]) - delta;
					double y = static_cast<double>(gradientRow[col][1]) - delta;
					magnitudeRow[col] = cv::saturate_cast<T>(sqrt(x * x + y * y));
				}
			}
		}
	};
}

#endif //FEATUREDETECTION_IMAGEPROCESSING_GRADIENTMAGNITUDEFILTER_HPP_
