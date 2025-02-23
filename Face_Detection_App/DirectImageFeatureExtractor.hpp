#pragma once
/*
* File: DirceImageFeatureExtractor.hpp
* Created on: 10/2/2025
* Author : Moin Shaikh
*/

#ifndef FEATUREDETECTION_IMAGEPROCESSING_DIRECTIMAGEFEATUREEXTRACTOR_HPP_
#define FEATUREDETECTION_IMAGEPROCESSING_DIRECTIMAGEFEATUREEXTRACTOR_HPP_


#include"FeatureExtractor.hpp"
#include"Version.hpp"

#include<memory>

namespace ImageProcessing
{
	class ImageFilter;
	class ChainedFilter;

	/**
	 * Feature extractor that extracts patches from images.
	 */
	class DirectImageFeatureExtractor : public FeatureExtractor {
	public:

		/**
		 * Constructs a new direct image feature extractor.
		 */
		DirectImageFeatureExtractor();

		/**
		 * Adds an image filter that is applied to the image.
		 *
		 * @param[in] filter The new image filter.
		 */
		void addImageFilter(std::shared_ptr<ImageFilter> filter);

		/**
		 * Adds a new filter that is applied to the patches.
		 *
		 * @param[in] filter The new patch filter.
		 */
		void addPatchFilter(std::shared_ptr<ImageFilter> filter);

		using FeatureExtractor::update;

		void update(std::shared_ptr<VersionedImage> image);

		std::shared_ptr<Patch> extract(int x, int y, int width, int height) const;

	private:

		Version version; ///< The version.
		cv::Mat image; ///< The filtered image.
		std::shared_ptr<ChainedFilter> imageFilter; ///< Filter that is applied to the image.
		std::shared_ptr<ChainedFilter> patchFilter; ///< Filter that is applied to the patches.
	};

}


#endif //FEATUREDETECTION_IMAGEPROCESSING_DIRECTIMAGEFEATUREEXTRACTOR_HPP_