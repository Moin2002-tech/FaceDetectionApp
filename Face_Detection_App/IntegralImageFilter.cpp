/*
* File: integralImageFilter.cpp
* Created on: 15/2/2025
* Author : Moin Shaikh
*/

#include "integralImageFilter.hpp"
#include<opencv2/imgproc.hpp>

using cv::Mat;

namespace ImageProcessing
{
	IntegralImageFilter::IntegralImageFilter(int type) : type(type) {}

	Mat IntegralImageFilter::applyTo(const Mat& image, Mat& filtered) const {
		cv::integral(image, filtered, type);
		return filtered;
	}
}