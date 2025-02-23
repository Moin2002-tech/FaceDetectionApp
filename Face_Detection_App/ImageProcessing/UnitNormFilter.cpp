/*
* File: UnitNormFilter.cpp
* Created on: 11/2/2025
* Author : Moin Shaikh
*/

#include"UnitNormFilter.hpp"

#include<stdexcept>

namespace ImageProcessing
{
	const float UnitNormFilter::eps = 1e-4;

	UnitNormFilter::UnitNormFilter(int normType) : normType(normType) 
	{

	}

	cv::Mat UnitNormFilter::applyTo(const cv::Mat &image,cv::Mat &filtered) const
	{
		if(image.channels() > 1)
		{
			throw std::invalid_argument("UnitNormFilter: The image must have exactly one channel.");
		}

		image.convertTo(filtered, CV_32F);
		normalize(filtered);
		return filtered;
	}

	void UnitNormFilter::applyInPlace(cv::Mat &image)const 
	{
		if(image.type() == CV_32FC1)
		{
			normalize(image);
		}
		else
		{
			image = applyTo(image);
		}
	}

	void UnitNormFilter::normalize(cv::Mat& image) const
	{
		double norm = cv::norm(image, normType);
		image = image / (norm + eps);
	}
}