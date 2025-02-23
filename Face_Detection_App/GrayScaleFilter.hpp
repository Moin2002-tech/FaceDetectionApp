#pragma once


/*
* File: GrayScaleFilter.hpp
* Created on: 10/2/2025
* Author : Moin Shaikh
*/


#ifndef FEATUREDETECTION_IMAGEPROCESSING_GRAYSCALEFILTER_HPP_
#define FEATUREDETECTION_IMAGEPROCESSING_GRAYSCALEFILTER_HPP_

#include"ImageFilter.hpp"
namespace ImageProcessing
{
	/**
 * Image filter that converts images to grayscale. If the input image does have only one channel, then the data
 * will be copied to the output image without any conversion. Otherwise, the image data is assumed to be BGR.
 */
	class GrayscaleFilter : public ImageFilter {
	public:

		/**
		 * Constructs a new grayscale filter.
		 */
		GrayscaleFilter();

		using ImageFilter::applyTo;

		cv::Mat applyTo(const cv::Mat& image, cv::Mat& filtered) const;

		void applyInPlace(cv::Mat& image) const;
	};
}

#endif //FEATUREDETECTION_IMAGEPROCESSING_GRAYSCALEFILTER_HPP_