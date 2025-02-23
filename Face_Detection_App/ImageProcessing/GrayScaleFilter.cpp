/*
* File: GrayScaleFilter.cpp
* Created on: 12/2/2025
* Author : Moin Shaikh
*/


#include"GrayScaleFilter.hpp"
#include<opencv2/imgproc.hpp>
#include<opencv2/core.hpp>



using cv::Mat;
using cv::cvtColor;

namespace ImageProcessing
{
	GrayscaleFilter::GrayscaleFilter()
	{

	}

	cv::Mat GrayscaleFilter::applyTo(const Mat &image, Mat &filtered) const
	{
		if(image.channels() > 1)
		{
			cvtColor(image, filtered, cv::COLOR_BGR2GRAY);
		}
		else 
		{
			image.copyTo(filtered);
		}
		return filtered;
	}

	void GrayscaleFilter::applyInPlace(Mat& image) const
	{
		if (image.channels() > 1)
		{
			Mat tmp;
			cvtColor(image, tmp, cv::COLOR_BGR2GRAY);
			image = tmp;
		}
	}
}
