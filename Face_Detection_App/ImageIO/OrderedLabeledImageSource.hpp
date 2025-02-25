/*
 * OrderedLabeledImageSource.hpp
 *
 *  Created on: 17/2/2025
 *      Author: Moin Shaikh
 */

#ifndef FEATUREDETECTION_IMAGEIO_ORDEREDLABELEDIMAGESOURCE_HPP_
#define FEATUREDETECTION_IMAGEIO_ORDEREDLABELEDIMAGESOURCE_HPP_

#include "LabeledImageSource.hpp"
#include "LandmarkSource.hpp"
#include <memory>

namespace imageio {

class OrderedLandmarkSource;

/**
 * Labeled image source whose landmarks are associated to the images by their order.
 */
class OrderedLabeledImageSource : public LabeledImageSource {
public:

	/**
	 * Constructs a new ordered labeled image source.
	 *
	 * @param[in] imageSource The source of the images.
	 * @param[in] landmarkSource The source of the landmarks.
	 */
	OrderedLabeledImageSource(std::shared_ptr<ImageSource> imageSource, std::shared_ptr<LandmarkSource> landmarkSource);

	~OrderedLabeledImageSource();

	void reset();

	bool next();

	const cv::Mat getImage() const;

	boost::filesystem::path getName() const;

	std::vector<boost::filesystem::path> getNames() const;

	const LandmarkCollection getLandmarks() const;

private:

	std::shared_ptr<ImageSource> imageSource;              ///< The source of the images.
	std::shared_ptr<LandmarkSource> landmarkSource; ///< The source of the landmarks.
};

} /* namespace imageio */
#endif /* ORDEREDLABELEDIMAGESOURCE_HPP_ */
