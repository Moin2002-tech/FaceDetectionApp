#pragma once

/*
* File: GrayWorldNormalizationFilter.hpp
* Created on: 10/2/2025
* Author : Moin Shaikh
*/
#ifndef FEATUREDETECTION_IMAGEPROCESSING_GREYWORLDNORMALIZATIONFILTER_HPP_
#define FEATUREDETECTION_IMAGEPROCESSING_GREYWORLDNORMALIZATIONFILTER_HPP_

#include"ImageFilter.hpp"


namespace ImageProcessing
{

	/**
	 * Image filter that does a grey world normalization. Expects the images to have three channels of uchar data
	 * (image type is CV_8UC3). The output image will have the same type.
	*/
	class GreyWorldNormalizationFilter : public ImageFilter 
	{
	public:

		/**
		 * Constructs a new grey world normalization filter.
		 */
		GreyWorldNormalizationFilter();

		using ImageFilter::applyTo;

		cv::Mat applyTo(const cv::Mat& image, cv::Mat& filtered) const;

		void applyInPlace(cv::Mat& image) const;
	};
}

#endif //FEATUREDETECTION_IMAGEPROCESSING_GREYWORLDNORMALIZATIONFILTER_HPP_