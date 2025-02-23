#pragma once
/*
* File: FeatureExtractor.hpp
* Created on: 4/2/2025
* Author : Moin Shaikh
*/


#ifndef FEATUREDETECTION_IMAGEPROCESSING_FEATUREEXTRACTOR_HPP_
#define FEATUREDETECTION_IMAGEPROCESSING_FEATUREEXTRACTOR_HPP_
#include<memory>

#include<opencv2/core.hpp>
#include"Patch.hpp"
#include"VersionedImage.hpp"

namespace ImageProcessing
{



	/**
		* Feature extractor that constructs a feature vector given an image and a rectangular size specifying a patch.
	*/
	class FeatureExtractor : public Patch
	{
	public:

		virtual ~FeatureExtractor() {}
		/**
		 * Forces an update of this feature extractor, so subsequent extracted features are based on the new image.
		 *
		 * @param[in] image The new source image of extracted patches.
		*/

		void update(const cv::Mat& image) {
			update(std::make_shared<VersionedImage>(image));
		}
		/**
		 * May update this feature extractor depending on the version number of the given image. If updated, the
		 * subsequent extracted features are based on the new image.
		 *
		 * @param[in] image The new source image of extracted patches.
		*/

		virtual void update(std::shared_ptr<VersionedImage> image) = 0;

		/**
		 * Extracts the feature vector of a certain location (patch) of the current image.
		 *
		 * @param[in] x The x-coordinate of the center position of the patch.
		 * @param[in] y The y-coordinate of the center position of the patch.
		 * @param[in] width The width of the patch.
		 * @param[in] height The height of the patch.
		 * @return A pointer to the patch (with its feature vector) that might be empty if the patch could not be created.
		*/
		virtual std::shared_ptr<Patch> extract(int x, int y, int width, int height) const = 0;
	};
}


#endif //FEATUREDETECTION_IMAGEPROCESSING_FEATUREEXTRACTOR_HPP_