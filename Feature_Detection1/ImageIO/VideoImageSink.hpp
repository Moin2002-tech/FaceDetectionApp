/*
 * VideoImageSink.hpp
 *
 *  Created on: 17/2/2025
 *      Author: Moin Shaikh
 */

#ifndef FEATUREDETECTION_IMAGEIO_VIDEOIMAGESINK_HPP_
#define FEATUREDETECTION_IMAGEIO_VIDEOIMAGESINK_HPP_

#include "ImageSink.hpp"
#include "opencv2/highgui/highgui.hpp"

namespace imageio {

/**
 * Image sink that stores images into a video file.
 */
class VideoImageSink : public ImageSink {
public:

	/**
	 * Constructs a new video image sink.
	 *
	 * @param[in] filename The name of the video file.
	 * @param[in] fps Framerate of the video stream.
	 * @param[in] fourcc 4-character code of video codec.
	 */
	explicit VideoImageSink(const std::string filename, double fps, int fourcc = cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));

	~VideoImageSink();

	void add(const cv::Mat& image);

private:
	const std::string filename; ///< The name of the video file.
	double fps;            ///< Framerate of the video stream.
	int fourcc;            ///< 4-character code of video codec.
	cv::VideoWriter writer;    ///< The video writer.
};

} /* namespace imageio */
#endif /* VIDEOIMAGESINK_HPP_ */
