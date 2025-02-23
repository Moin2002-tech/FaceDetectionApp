#pragma once
/*
* File: ChannelExtractor.hpp
* Created on: 10/2/2025
* Author : Moin Shaikh
*/

#ifndef FEATUREDETECTION_IMAGEPROCESSING_FILTERING_CHANNELEXTRACTOR_HPP_
#define FEATUREDETECTION_IMAGEPROCESSING_FILTERING_CHANNELEXTRACTOR_HPP_

#include"../../ImageProcessing/ImageFilter.hpp"
namespace ImageProcessing
{
	namespace Filtering
	{
		/**
		 * Image filter that extracts channels from the original image.
		*/
		class ChannelExtractor : public ImageFilter {
		public:

			/**
			 * Constructs a new channel selector that selects a single channel.
			 *
			 * @param[in] channel Channel that should be selected.
			 */
			explicit ChannelExtractor(int channel);

			/**
			 * Constructs a new channel selector.
			 *
			 * @param[in] channels Channels that should be extracted in that order.
			 */
			explicit ChannelExtractor(const std::vector<int>& channels);

			using ImageFilter::applyTo;

			cv::Mat applyTo(const cv::Mat& image, cv::Mat& filtered) const;

		private:

			void createFromTo(const std::vector<int>& channels);

			std::vector<int> fromTo; ///< Channel index pairs specifying input (2k) and output (2k+1) channel indices.
		};
	}

}
#endif 
