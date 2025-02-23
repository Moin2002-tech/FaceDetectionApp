/*
* File: ColorSpaceConversionFilter.hpp
* Created on: 20/2/2025
* Author : Moin Shaikh
*/


#include "ColorSpaceConversionFilter.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using cv::Mat;

namespace ImageProcessing
{
	ColorSpaceConversionFilter::ColorSpaceConversionFilter(int conversion) : conversion(conversion) 
	{

	}

	Mat ColorSpaceConversionFilter::applyTo(const Mat& image, Mat& filtered) const 
	{
		cv::cvtColor(image, filtered, conversion);
		return filtered;
	}
}