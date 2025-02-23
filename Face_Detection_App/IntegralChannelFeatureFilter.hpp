#pragma once
/*
* File: IntegralChannelFeatureFilter.hpp
* Created on: 10/2/2025
* Author : Moin Shaikh
*/

#ifndef FEATUREDETECTION_IMAGEPROCESSING_INTEGRALCHANNELFEATUREFILTER_HPP_
#define FEATUREDETECTION_IMAGEPROCESSING_INTEGRALCHANNELFEATUREFILTER_HPP_

#include"ImageFilter.hpp"

namespace ImageProcessing
{
	/**
	 * Filter that computes first-order integral channel features (sums over rectangular regions). The input images are
	 * supposed to have multiple integral channels with depth CV_32S. The output will will contain the average value
	 * over each of the rectangular regions and have a depth of CV_8U.
	*/
	class IntegralChannelFeatureFilter : public ImageFilter {
	public:

		/**
		 * Constructs a new integral channel feature filter.
		 *
		 * @param[in] blockRows The amount of rows of blocks.
		 * @param[in] blockColumns The amount of columns of blocks.
		 * @param[in] overlap The overlap between neighboring blocks.
		 */
		IntegralChannelFeatureFilter(unsigned int blockRows, unsigned int blockColumns, double overlap = 0);

		using ImageFilter::applyTo;

		cv::Mat applyTo(const cv::Mat& image, cv::Mat& filtered) const;

	private:

		unsigned int blockRows;    ///< The amount of rows of blocks.
		unsigned int blockColumns; ///< The amount of columns of blocks.
		double overlap; ///< The overlap between neighboring blocks.
	};
}

#endif //FEATUREDETECTION_IMAGEPROCESSING_INTEGRALCHANNELFEATUREFILTER_HPP_