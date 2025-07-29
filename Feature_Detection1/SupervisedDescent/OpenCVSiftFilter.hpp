#pragma once
/*
* File: OpenCVSiftFilter.hpp
* Created on: 17/7/2025
* Author : Moin Shaikh
*/



#ifndef FEATUREDETECTION_SUPERVISEDDESCENT_OPENCVSIFTFILTER_HPP_
#define FEATUREDETECTION_SUPERVISEDDESCENT_OPENCVSIFTFILTER_HPP_


#include<opencv2/features2d.hpp>

#include"../ImageProcessing/ImageFilter.hpp"

namespace superVisedDescent
{
	/**
	 * Filter that uses cv::SIFT to extract SIFT features of the given image
	 * patch. No keypoint detection is performed. The center of the patch is
	 * used as the center of where to extract the features.
	*/

	class OpenCVSiftFilter : public ImageProcessing::ImageFilter
	{
	public:
		OpenCVSiftFilter(cv::Ptr<cv::SIFT> sift);

		using ImageFilter::applyTo;
		cv::Mat applyTo(const cv::Mat& image, cv::Mat& filtered) const;

	private:
		cv::Ptr<cv::SIFT> sift;
	};
}


#endif //FEATUREDETECTION_SUPERVISEDDESCENT_OPENCVSIFTFILTER_HPP_