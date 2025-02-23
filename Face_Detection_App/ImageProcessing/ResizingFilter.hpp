#pragma once
/*
* File: ResizingFilter.hpp
* Created on: 9/2/2025
* Author : Moin Shaikh
*/


#ifndef	FEATUREDETECTION_IMAGEPROCESSING_RESIZINGFILTER_HPP_
#define FEATUREDETECTION_IMAGEPROCESSING_RESIZINGFILTER_HPP_
#include<opencv2/imgproc.hpp>
#include"ImageFilter.hpp"

namespace ImageProcessing
{
	/**
		* Filter that resizes images to a certain size.
 */
	class ResizingFilter : public ImageFilter 
	{
	public:

		/**
		 * Constructs a new resizing filter.
		 *
		 * @param[in] size The size of the filtered images.
		 * @param[in] interpolation The interpolation method (see last parameter of cv::resize).
		 */
		explicit ResizingFilter(cv::Size size, int interpolation = cv::INTER_AREA);

		using ImageFilter::applyTo;

		cv::Mat applyTo(const cv::Mat& image, cv::Mat& filtered) const;

	private:

		cv::Size size;     ///< The size of the filtered images.
		int interpolation; ///< The interpolation method.
	};
}

#endif //FEATUREDETECTION_IMAGEPROCESSING_RESIZINGFILTER_HPP_