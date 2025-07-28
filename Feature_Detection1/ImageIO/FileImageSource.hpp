/*
 * FileImageSource.hpp
 *
 *  Created on: 17/2/2025
 *      Author: Moin Shaikh
 */

#ifndef FEATUREDETECTION_IMAGEIO_FILEIMAGESOURCE_HPP_
#define FEATUREDETECTION_IMAGEIO_FILEIMAGESOURCE_HPP_

#include "ImageSource.hpp"

#include "boost/filesystem.hpp"
#include "opencv2/core/core.hpp"
#include <vector>

namespace imageio {

/**
 * Image source that takes a single image or a vector of images.
 */
class FileImageSource : public ImageSource {
public:

	/**
	 * Constructs a new file image source from a single file.
	 *
	 * @param[in] filePath The path and filename of the image.
	 */
	explicit FileImageSource(const std::string& filePath);

	/**
	 * Constructs a new file image source from a vector of files.
	 *
	 * @param[in] filePaths A vector with all the filenames.
	 */
	explicit FileImageSource(std::vector<std::string> filePaths);

	~FileImageSource();

	void reset();

	bool next();

	const cv::Mat getImage() const;

	boost::filesystem::path getName() const;

	std::vector<boost::filesystem::path> getNames() const;

private:
	std::vector<boost::filesystem::path> files; ///< The files of the given directory, ordered by name.
	int index; ///< The index of the next file.

};

} /* namespace imageio */
#endif /* FILEIMAGESOURCE_HPP_ */
