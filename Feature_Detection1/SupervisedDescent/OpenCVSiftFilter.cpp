/*
* File: OpenCVSiftFilter.cpp
* Created on: 19/7/2025
* Author : Moin Shaikh
*/


#include "OpenCVSiftFilter.hpp"

#include<opencv2/imgproc.hpp>

#include<vector>

using cv::Mat;
using std::vector;

namespace superVisedDescent
{
	OpenCVSiftFilter::OpenCVSiftFilter(cv::Ptr<cv::SIFT> sift) : sift(sift)
	{

	}
	cv::Mat OpenCVSiftFilter::applyTo(const cv::Mat &image, cv::Mat &filtered) const
	{
		Mat grayScaledImage;
		if(image.channels() == 3)
		{
			cv::cvtColor(image, grayScaledImage, cv::COLOR_BGR2GRAY);
		}
		else
		{
			grayScaledImage = image;
		}
		cv::Ptr<cv::SIFT> sift = cv::SIFT::create();
		std::vector<cv::KeyPoint> keyPoints;

		
		sift->compute(grayScaledImage, keyPoints, filtered);

		return filtered;
	}
}
