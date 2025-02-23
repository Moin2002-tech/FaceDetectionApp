#pragma once
/*
* File: BgrToLuvConverter.hpp
* Created on: 10/2/2025
* Author : Moin Shaikh
*/
#ifndef FEATUREDETECTION_IMAGEPROCESSING_FILTERING_BGRTOLUVCONVERTER_HPP_
#define FEATUREDETECTION_IMAGEPROCESSING_FILTERING_BGRTOLUVCONVERTER_HPP_


#include"../../ImageProcessing/ImageFilter.hpp"
#include<vector>

namespace ImageProcessing 
{
	namespace Filtering
	{
		/**
		 * Filter that converts images from the BGR color space to CIE L*u*v* and normalizes the output to be within [0,1].
		 *
		 * According to OpenCV's documentation, the value ranges are [0,100] for L, [-134,220] for u and [-140,122] for v.
		 * To maintain the perceptual uniformity, all values are scaled by the same factor (354). The value ranges then are
		 * [0,0.28] for L, [0,1] for u and [0,0.74] for v. If the perceptual uniformity should not be maintained, all the
		 * values are scaled to be within [0,1].
		 *
		 * Input images are expected to be in BGR format and must be of type CV_8U or CV_32FC3 with their values scaled to
		 * in [0,1]. The resulting image will be of type CV_32FC3.
		*/
		class BgrToLuvConverter : public ImageFilter {
		public:

			/**
			 * Constructs a new BGR to L*u*v* converter.
			 *
			 * @param[in] maintainPerceptualUniformity Flag that indicates whether to maintain perceptual uniformity.
			 */
			explicit BgrToLuvConverter(bool maintainPerceptualUniformity = true);

			using ImageFilter::applyTo;

			cv::Mat applyTo(const cv::Mat& image, cv::Mat& filtered) const;

		private:

			void createLuvLut();

			void createBgrFloat(cv::Mat& bgrImage);

			void convertToNormalizedLuvUchar(const cv::Mat& image, cv::Mat& filtered) const;

		public:

			void convertToNormalizedLuv(const cv::Vec3b& bgr, cv::Vec3f& luv) const;

		private:

			void convertToLuvFloat(const cv::Mat& image, cv::Mat& filtered) const;

			void normalizeLuv(cv::Mat& image) const;

			void normalize(cv::Vec3f& luv) const;

			bool maintainPerceptualUniformity; ///< Flag that indicates whether to maintain perceptual uniformity.
			cv::Mat luvLut; ///< Look-up table of the L*u*v* colors given a BGR-based index.
		};

	}
}

#endif //FEATUREDETECTION_IMAGEPROCESSING_FILTERING_BGRTOLUVCONVERTER_HPP_
