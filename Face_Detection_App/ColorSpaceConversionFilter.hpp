#pragma once
/*
* File: ColorSpaceConversionFilter.hpp
* Created on: 10/2/2025
* Author : Moin Shaikh
*/


#ifndef FEATUREDETECTION_IMAGEPROCESSING_COLORSPACECONVERSIONFILTER_HPP_
#define FEATUREDETECTION_IMAGEPROCESSING_COLORSPACECONVERSIONFILTER_HPP_

#include"ImageFilter.hpp"

namespace ImageProcessing
{
	/**
	 * Image filter that converts images from one color space to another.
	 */
	class ColorSpaceConversionFilter : public ImageFilter
	{
	public:

		/**
		 * Constructs a new color space conversion filter.
		 *
		 * @param[in] conversion The conversion code, see cv::cvtColor for details.
		 */
		explicit ColorSpaceConversionFilter(int conversion);

		using ImageFilter::applyTo;

		cv::Mat applyTo(const cv::Mat& image, cv::Mat& filtered) const;

	private:

		int conversion; ///< The conversion code, see cv::cvtColor for details.
	};
}

#endif //FEATUREDETECTION_IMAGEPROCESSING_COLORSPACECONVERSIONFILTER_HPP_