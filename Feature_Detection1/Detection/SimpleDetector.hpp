

#ifndef FEATUREDETECTION_DETECTION_SIMPLEDETECTOR_HPP_
#define FEATUREDETECTION_DETECTION_SIMPLEDETECTOR_HPP_

#include "../ImageProcessing/VersionedImage.hpp"
#include "opencv2/core/core.hpp"
#include <memory>
#include <vector>

namespace detection {

/**
 * Detector that finds instances of a common type inside images and returns bounding boxes around those.
 */
class SimpleDetector {
public:

	virtual ~SimpleDetector() {}

	/**
	 * Detects objects inside the given image.
	 *
	 * @param[in] image Image to find objects inside.
	 * @return Bounding boxes around the found objects.
	 */
	std::vector<cv::Rect> detect(const cv::Mat& image) {
		return detect(std::make_shared<ImageProcessing::VersionedImage>(image));
	}


	/**
	 * Detects objects inside the given image.
	 *
	 * @param[in] image Image to find objects inside.
	 * @return Bounding boxes around the found objects.
	 */
	virtual std::vector<cv::Rect> detect(std::shared_ptr<ImageProcessing::VersionedImage> image) = 0;

	/**
	 * Detects objects inside the given image and returns their positions and scores.
	 *
	 * @param[in] image Image to find objects inside.
	 * @return Bounding boxes around the found objects with their score, ordered by score in descending order.
	 */
	std::vector<std::pair<cv::Rect, float>> detectWithScores(const cv::Mat& image) {
		return detectWithScores(std::make_shared<ImageProcessing::VersionedImage>(image));
	}

	/**
	 * Detects objects inside the given image and returns their positions and scores.
	 *
	 * @param[in] image Image to find objects inside.
	 * @return Bounding boxes around the found objects with their score, ordered by score in descending order.
	 */
	virtual std::vector<std::pair<cv::Rect, float>> detectWithScores(std::shared_ptr<ImageProcessing::VersionedImage> image) = 0;
};

} /* namespace detection */

#endif
