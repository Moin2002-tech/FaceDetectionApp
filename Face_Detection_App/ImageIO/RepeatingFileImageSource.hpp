/*
 * RepeatingFileImageSource.hpp
 *
 *  Created on: 17/2/2025
 *      Author: Moin Shaikh
 */

#ifndef FEATUREDETECTION_IMAGEIO_REPEATINGFILEIMAGESOURCE_HPP_
#define FEATUREDETECTION_IMAGEIO_REPEATINGFILEIMAGESOURCE_HPP_

#include "ImageSource.hpp"

#include "boost/filesystem.hpp"
#include "opencv2/core/core.hpp"
#include <vector>

using boost::filesystem::path;
using std::string;
using cv::Mat;
using std::vector;

namespace imageio {

/**
 * Image source that takes a single image and repeatedly outputs it.
 */
class RepeatingFileImageSource : public ImageSource {
public:

	/**
	 * Constructs a new repeating file image source.
	 *
	 * @param[in] file The path and filename of the image.
	 */
	RepeatingFileImageSource(string filePath);

	virtual ~RepeatingFileImageSource();

	void reset();

	bool next();

	const Mat getImage() const;

	path getName() const;

	vector<path> getNames() const;

private:
	path file; ///< The path to the file that is output repeatedly.
	Mat image; ///< The image. As it's only one image, we pre-load it.
};

} /* namespace imageio */
#endif /* REPEATINGFILEIMAGESOURCE_HPP_ */
