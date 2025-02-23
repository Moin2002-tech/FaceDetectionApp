/*
* File: ConversionFilter.hpp
* Created on: 20/2/2025
* Author : Moin Shaikh
*/

#include"ConversionFilter.hpp"
#include<stdexcept>

using cv::Mat;
using std::invalid_argument;

namespace ImageProcessing
{
	ConversionFilter::ConversionFilter(int type, double alpha, double beta) : type(type), alpha(alpha), beta(beta) 
	{

	}

	Mat ConversionFilter::applyTo(const Mat& image, Mat& filtered) const
	{
		image.convertTo(filtered, type, alpha, beta);
		return filtered;
	}

	void ConversionFilter::applyInPlace(Mat& image) const 
	{
		applyTo(image, image);
	}
}