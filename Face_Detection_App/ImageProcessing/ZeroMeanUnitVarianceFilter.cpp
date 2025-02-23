/*
 * ZeroMeanUnitVarianceFilter.hpp
 *
 *  Created on: 11/2/2025
 *     Author: Moin Shaikh
*/


#include"ZeroMeanVarianceFilter.hpp"
#include<stdexcept>

namespace ImageProcessing
{

	ZeroMeanUnitVarianceFilter::ZeroMeanUnitVarianceFilter() {}

	cv::Mat ZeroMeanUnitVarianceFilter::applyTo(const cv::Mat& image,cv::Mat& filtered) const
	{
		if(image.channels() > 1 )
		{
			throw std::invalid_argument("ZeroMeanUnitVarianceFilter: The image must have exactly one channel.");
		}

		cv::Scalar mean, deviation;
		cv::meanStdDev(image, mean, deviation);
		image.convertTo(image, CV_32F);

		if(deviation[0] == 0)
		{
			filtered = cv::Scalar(0);
		}
		else
		{
			filtered = (filtered - mean) / deviation[0];
		}

		return filtered;
	}
}
