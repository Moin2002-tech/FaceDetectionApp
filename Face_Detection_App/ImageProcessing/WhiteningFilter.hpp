#pragma once
/*
* File: WhiteningFitler.hpp
* Created on: 9/2/2025
* Author : Moin Shaikh
*/

#ifndef FEATUREDETECTION_IMAGEPROCESSING_WHITENINGFILTER_HPP_
#define FEATUREDETECTION_IMAGEPROCESSING_WHITENINGFILTER_HPP_

#include"ImageFilter.hpp"

namespace ImageProcessing
{
	/**
	 * Image filter that approximately whitens the power spectrum of natural images. The input image
	 * must not have more than one channel and can be of any type. The output image has the same type
	 * as the input image.
	 * 
	*/

	class WhiteningFilter : public ImageFilter {
	public:

		/**
		 * Constructs a new whitening filter.
		 *
		 * @param[in] alpha Decay of modulus of spectrum is assumed as 1/frequency^alpha.
		 * @param[in] cutoffFrequency The cut-off frequency of the additional low-pass filter (only applied when greater than zero).
		 */
		WhiteningFilter(float alpha = 1, float cutoffFrequency = 0.390625);

		using ImageFilter::applyTo;

		cv::Mat applyTo(const cv::Mat& image, cv::Mat& filtered) const;

		void applyInPlace(cv::Mat& image) const;

	private:

		/**
		 * Provides the whitening filter for the image in the frequency domain.
		 *
		 * @param[in] width The width of the image (and filter).
		 * @param[in] height The height of the image (and filter).
		 * @return The filter of the given size.
		 */
		const cv::Mat& getFilter(int width, int height) const;

		float alpha;           ///< Decay of modulus of spectrum is assumed as 1/frequency^alpha.
		float cutoffFrequency; ///< The cut-off frequency of the additional low-pass filter (only applied when greater than zero).
		mutable cv::Mat filter;       ///< The current filter.
		mutable cv::Mat floatImage;   ///< Temporal buffer for the float conversion of the image.
		mutable cv::Mat fourierImage; ///< Temporal buffer for the Fourier transformation of the image.
	};

}

#endif //FEATUREDETECTION_IMAGEPROCESSING_WHITENINGFILTER_HPP_