#pragma once
/*
* File: ConvolutionFilter.hpp
* Created on: 10/2/2025
* Author : Moin Shaikh
*/

#ifndef FEATUREDETECTION_IMAGEPROCESSING_CONVOLUTIONFILTER_HPP_
#define FEATUREDETECTION_IMAGEPROCESSING_CONVOLUTIONFILTER_HPP_




#include"ImageFilter.hpp"

namespace ImageProcessing
{
	/**
	 * Filter that convolves the image with a kernel.
	 */
	class ConvolutionFilter : public ImageFilter {
	public:

		/**
		 * Constructs a new convolution filter.
		 *
		 * @param[in] kernel The convolution kernel.
		 * @param[in] anchor The anchor point within the kernel, (-1, -1) means the kernel center.
		 * @param[in] delta The value that is added to each pixel after the convolution.
		 * @param[in] depth The desired depth of the filted image.
		 */
		explicit ConvolutionFilter(const cv::Mat& kernel, cv::Point anchor = cv::Point(-1, -1), double delta = 0, int depth = -1);

		/**
		 * Constructs a new convolution filter with an empty kernel.
		 *
		 * @param[in] depth The desired depth of the filted image.
		 */
		explicit ConvolutionFilter(int depth = -1);

		using ImageFilter::applyTo;

		cv::Mat applyTo(const cv::Mat& image, cv::Mat& filtered) const;

		/**
		 * Changes the convolution kernel.
		 *
		 * @param[in] kernel The new convolution kernel.
		 */
		void setKernel(const cv::Mat& kernel);

		/**
		 * Changes the anchor point within the kernel.
		 *
		 * @param[in] anchor The anchor point within the kernel, (-1, -1) means the kernel center.
		 */
		void setAnchor(cv::Point anchor);

		/**
		 * Changes the value that is being added to each pixel after the convolution.
		 *
		 * @param[in] delta The new value that is added to each pixel after the convolution.
		 */
		void setDelta(double delta);

	private:

		std::vector<cv::Mat> kernels; ///< The kernels per channel.
		cv::Point anchor; ///< The anchor point within the kernels.
		double delta; ///< The value that is added to the convolution result.
		int depth; ///< The desired depth of the filted image.
	};


}




#endif //FEATUREDETECTION_IMAGEPROCESSING_CONVOLUTIONFILTER_HPP_
