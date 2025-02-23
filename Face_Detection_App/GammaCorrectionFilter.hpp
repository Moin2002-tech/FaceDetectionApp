#pragma once



/*
* File: GammaCorrectionFilter.hpp
* Created on: 10/2/2025
* Author : Moin Shaikh
*/

#ifndef FEATUREDETECTION_IMAGEPROCESSING_GAMMACORRECTIONFILTER_HPP_
#define FEATUREDETECTION_IMAGEPROCESSING_GAMMACORRECTIONFILTER_HPP_

#include"ImageFilter.hpp"

namespace ImageProcessing
{
	class GammaCorrectionFilter : public ImageFilter
	{
	public:

		/**
		 * Constructs a new gamma correction filter.
		 *
		 * @param[in] gamma The power of the intensity. Must be greater than zero.
		 */
		explicit GammaCorrectionFilter(double gamma);

		using ImageFilter::applyTo;

		cv::Mat applyTo(const cv::Mat& image, cv::Mat& filtered) const;

		void applyInPlace(cv::Mat& image) const;

	private:

		template<class T>
		cv::Mat applyGammaCorrection(const cv::Mat& image, cv::Mat& filtered) const {
			int rows = image.rows;
			int cols = image.cols;
			filtered.create(image.rows, image.cols, image.type());
			if (image.isContinuous() && filtered.isContinuous()) {
				cols *= rows;
				rows = 1;
			}
			for (int row = 0; row < rows; ++row) {
				const T* originalRow = image.ptr<T>(0);
				T* filteredRow = filtered.ptr<T>(0);
				for (int col = 0; col < cols; ++col)
					filteredRow[col] = pow(originalRow[col], gamma);
			}
			return filtered;
		}

		double gamma; ///< The power of the intensity. Must be greater than zero.
		cv::Mat lut; ///< Look-up table of gamma corrected uchar values.
	};

}


#endif //FEATUREDETECTION_IMAGEPROCESSING_GAMMACORRECTIONFILTER_HPP_
