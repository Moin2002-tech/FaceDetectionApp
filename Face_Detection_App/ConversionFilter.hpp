#pragma once


/*
* File: ConversionFilter.hpp
* Created on: 10/2/2025
* Author : Moin Shaikh
*/


#ifndef FEATUREDETECTION_IMAGEPROCESSING_CONVERSIONFILTER_HPP_
#define FEATUREDETECTION_IMAGEPROCESSING_CONVERSIONFILTER_HPP_



#include"ImageFilter.hpp"


namespace ImageProcessing
{
	/**
	 * Filter that converts the image to another type, rescaling the values. The amount of channels will remain unchanged.
	*/
	class ConversionFilter : public ImageFilter {
	public:

		/**
		 * Constructs a new conversion filter.
		 *
		 * @param[in] type The OpenCV type (depth) of the filtered images. If negative, the type remains unchanged.
		 * @param[in] alpha The optional scaling factor.
		 * @param[in] beta The optional delta that is added to the scaled values.
		 */
		explicit ConversionFilter(int type, double alpha = 1, double beta = 0);

		using ImageFilter::applyTo;

		cv::Mat applyTo(const cv::Mat& image, cv::Mat& filtered) const;

		void applyInPlace(cv::Mat& image) const;

	private:

		int type;     ///< The type (depth) of the filtered images.
		double alpha; ///< The scaling factor.
		double beta;  ///< The delta that is added to the scaled values.
	};
}

#endif //FEATUREDETECTION_IMAGEPROCESSING_CONVERSIONFILTER_HPP_

