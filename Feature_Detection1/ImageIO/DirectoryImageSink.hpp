/*
 * DirectoryImageSink.hpp
 *
 *  Created on: 17/2/2025
 *      Author: Moin Shaikh
 */

#ifndef FEATUREDETECTION_IMAGEIO_DIRECTORYIMAGESINK_HPP_
#define FEATUREDETECTION_IMAGEIO_DIRECTORYIMAGESINK_HPP_

#include "ImageSink.hpp"

namespace imageio {

/**
 * Image sink that stores images into a directory.
 */
class DirectoryImageSink : public ImageSink {
public:

	/**
	 * Constructs a new directory image sink.
	 *
	 * @param[in] directory The name of the directory.
	 * @param[in] ending The file ending of the image files.
	 */
	explicit DirectoryImageSink(std::string directory, std::string ending = "png");

	~DirectoryImageSink();

	void add(const cv::Mat& image);

private:

	std::string directory;   ///< The name of the directory.
	std::string ending;      ///< The file ending of the image files.
	unsigned int index; ///< The index of the next file.
};

} /* namespace imageio */
#endif /* DIRECTORYIMAGESINK_HPP_ */
