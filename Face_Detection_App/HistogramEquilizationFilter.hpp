#pragma once

/*
* File: HistogramEquilizationFilter.hpp
* Created on: 10/2/2025
* Author : Moin Shaikh
*/


#ifndef FEATUREDETECTION_IMAGEPROCESSING_HISTOGRAMEQUILIZATIONFILTER_HPP_
#define FEATUREDETECTION_IMAGEPROCESSING_HISTOGRAMEQUILIZATIONFILTER_HPP_


#include"ImageFilter.hpp"

namespace ImageProcessing
{
	/**
	 * Image filter that equalizes the histogram using OpenCV's equalizeHist function. The images must
	 * be of type 8-bit single channel (CV_8U / CV_8UC1). The output image will be CV_8U as well.
	*/
	class HistogramEqualizationFilter : public ImageFilter {
	public:

		/**
		 * Constructs a new histogram equalization filter.
		 */
		HistogramEqualizationFilter();

		using ImageFilter::applyTo;

		cv::Mat applyTo(const cv::Mat& image, cv::Mat& filtered) const;

		void applyInPlace(cv::Mat& image) const;
	};

}

#endif //FEATUREDETECTION_IMAGEPROCESSING_HISTOGRAMEQUILIZATIONFILTER_HPP_
