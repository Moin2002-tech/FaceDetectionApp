

/*
* File: ResizingFilter.cpp
* Created on: 15/2/2025
* Author : Moin Shaikh
*/



#include"ResizingFilter.hpp"

using cv::Mat;
using cv::Size;
using cv::resize;

namespace ImageProcessing
{
	ResizingFilter::ResizingFilter(Size size, int interpolation) : 
		size(size), interpolation(interpolation) 
	{ 
	}

	Mat ResizingFilter::applyTo(const Mat& image, Mat& filtered) const 
	{
		resize(image, filtered, size, 0, 0, interpolation);
		return filtered;
	}
}
