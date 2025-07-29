/*
 * ImageSink.hpp
 *
 *  Created on: 17/2/2025
 *      Author: Moin Shaikh
 */

#ifndef FEATUREDETECTION_IMAGEIO_IMAGESINK_HPP_
#define FEATUREDETECTION_IMAGEIO_IMAGESINK_HPP_

#include "opencv2/core/core.hpp"

namespace imageio {

/**
 * Sink for subsequent images.
 */
class ImageSink {
public:

	virtual ~ImageSink() {}

	/**
	 * Adds an image.
	 *
	 * @param[in] image The image.
	 */
	virtual void add(const cv::Mat& image) = 0;
};

} /* namespace imageio */
#endif /* IMAGESINK_HPP_ */
