#pragma once
/*
* File: IntegralChannelHistogramFilter.hpp
* Created on: 10/2/2025
* Author : Moin Shaikh
*/

#ifndef FEATUREDETECTION_IMAGEPROCESSING_INTEGRALCHANNELHISTOGRAMFILTER_HPP_
#define FEATUREDETECTION_IMAGEPROCESSING_INTEGRALCHANNELHISTOGRAMFILTER_HPP_

#include"HistogramFilter.hpp"


namespace ImageProcessing
{
	/**
 * Filter that expects each channel of the given images to be an integral histogram bin. The filter result is a
 * vector of concatenated normalized histograms over overlapping blocks. The input images must have a depth of
 * CV_32S, the output vectors will have a depth of CV_32F.
 */
	class IntegralChannelHistogramFilter : public HistogramFilter 
	{
	public:

		/**
		 * Constructs a new integral channel histogram filter.
		 *
		 * @param[in] blockRows The amount of rows of blocks.
		 * @param[in] blockColumns The amount of columns of blocks.
		 * @param[in] overlap The overlap between neighboring blocks.
		 * @param[in] normalization The normalization method of the histograms.
		 */
		IntegralChannelHistogramFilter(unsigned int blockRows, unsigned int blockColumns, double overlap = 0,
			Normalization normalization = Normalization::L2NORM);

		using ImageFilter::applyTo;

		cv::Mat applyTo(const cv::Mat& image, cv::Mat& filtered) const;

	private:

		unsigned int blockRows;    ///< The amount of rows of blocks.
		unsigned int blockColumns; ///< The amount of columns of blocks.
		double overlap; ///< The overlap between neighboring blocks.
	};
}

#endif //FEATUREDETECTION_IMAGEPROCESSING_INTEGRALCHANNELHISTOGRAMFILTER_HPP_