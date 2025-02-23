#pragma once
/*
* File: IntegralFeatureExtractor.hpp
* Created on: 10/2/2025
* Author : Moin Shaikh
*/

#ifndef FEATUREDETECTION_IMAGEPROCESSING_INTEGRALFEATUREEXTRACTOR_HPP_
#define FEATUREDETECTION_IMAGEPROCESSING_INTEGRALFEATUREEXTRACTOR_HPP_

#include"FeatureExtractor.hpp"
#include"Patch.hpp"

namespace ImageProcessing
{
	/**
	 * Feature extractor based on integral images. Because integral images are one pixel wider and higher, the size
	 * of the patch request is increased by one to include another row and column to the bottom and right of the patch.
	*/
	class IntegralFeatureExtractor : public FeatureExtractor
	{
	public:

		/**
		 * Constructs a new integral feature extractor.
		 *
		 * @param[in] extractor The underlying feature extractor.
		 */
		explicit IntegralFeatureExtractor(std::shared_ptr<FeatureExtractor> extractor) : extractor(extractor) 
		{}

		using FeatureExtractor::update;

		void update(std::shared_ptr<VersionedImage> image) {
			extractor->update(image);
		}

		std::shared_ptr<Patch> extract(int x, int y, int width, int height) const
		{
			int offsetX = width % 2 == 0 ? 0 : 1;
			int offsetY = height % 2 == 0 ? 0 : 1;
			std::shared_ptr<Patch> patch = extractor->extract(x + offsetX, y + offsetY, width + 1, height + 1);
			if (patch) {
				patch->setX(patch->getX() - offsetX);
				patch->setY(patch->getY() - offsetY);
				patch->setWidth(patch->getWidth() - 1);
				patch->setHeight(patch->getHeight() - 1);
			}
			return patch;
		}

	private:

		std::shared_ptr<FeatureExtractor> extractor; ///< The underlying feature extractor.
	};
}


#endif //FEATUREDETECTION_IMAGEPROCESSING_INTEGRALFEATUREEXTRACTOR_HPP_