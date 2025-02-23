/*
* File: ReshapingFitler.cpp
* Created on: 15/2/2025
* Author : Moin Shaikh
*/

#include"ReshapingFilter.hpp"

using cv::Mat;
using std::vector;
using std::shared_ptr;

namespace ImageProcessing
{
	ReshapingFilter::ReshapingFilter(int rows, int channels) :
		rows(rows), channels(channels) 
	{

	}

	Mat ReshapingFilter::applyTo(const Mat& image, Mat& filtered) const
	{
		image.copyTo(filtered);
		filtered = filtered.reshape(channels, rows);
		return filtered;
	}

	void ReshapingFilter::applyInPlace(Mat& image) const
	{
		applyTo(image, image);
	}
}
