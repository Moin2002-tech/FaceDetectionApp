/*
* File: HistogramEqualizationFilter.cpp
* Created on: 15/2/2025
* Author : Moin Shaikh
*/

#include"HistogramEquilizationFilter.hpp"

#include<opencv2/imgproc.hpp>

using cv::Mat;

namespace ImageProcessing
{
	HistogramEqualizationFilter::HistogramEqualizationFilter() 
	{

	}

	Mat HistogramEqualizationFilter::applyTo(const Mat& image, Mat& filtered) const
	{
		cv::equalizeHist(image, filtered);
		return filtered;
	}

	void HistogramEqualizationFilter::applyInPlace(Mat& image) const 
	{
		applyTo(image, image);
	}

}