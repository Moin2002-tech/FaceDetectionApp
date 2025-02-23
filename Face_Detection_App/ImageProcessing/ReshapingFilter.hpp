#pragma once
/*
* File: ReshapingFilter.hpp
* Created on: 9/2/2025
* Author : Moin Shaikh
*/

#ifndef FEATUREDETECTION_IMAGEPROCESSING_RESHAPINGFILTER_HPP_
#define FEATUREDETECTION_IMAGEPROCESSING_RESHAPINGFILTER_HPP_

#include "ImageFilter.hpp"

namespace ImageProcessing
{
	/**
		* Filter that reshapes images.
	*/
	class ReshapingFilter : public ImageFilter
	{
	public:

		/**
		 * Constructs a new reshaping filter.
		 *
		 * @param[in] rows The new number of rows. If zero, the number of rows remains the same.
		 * @param[in] channels The new number of channels. If zero, the number of channels remains the same.
		 */
		explicit ReshapingFilter(int rows, int channels = 0);

		using ImageFilter::applyTo;

		cv::Mat applyTo(const cv::Mat& image, cv::Mat& filtered) const;

		void applyInPlace(cv::Mat& image) const;

	private:

		int rows;     ///< The new number of rows. If zero, the number of rows remains the same.
		int channels; ///< The new number of channels. If zero, the number of channels remains the same.
	};
}

#endif //FEATUREDETECTION_IMAGEPROCESSING_RESHAPINGFILTER_HPP_