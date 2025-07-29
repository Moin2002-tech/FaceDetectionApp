/*
 * DirectoryImageSource.hpp
 *
 *  Created on: 17/2/2025
 *      Author: Moin Shaikh
 */

#ifndef FEATUREDETECTION_IMAGEIO_DIRECTORYIMAGESOURCE_HPP_
#define FEATUREDETECTION_IMAGEIO_DIRECTORYIMAGESOURCE_HPP_

#include "ImageSource.hpp"

#include "boost/filesystem.hpp"
#include "opencv2/core/core.hpp"
#include <vector>

using boost::filesystem::path;
using cv::Mat;
using std::vector;
using std::string;

namespace imageio {

/**
 * Image source that takes the images of a directory.
 */
class DirectoryImageSource : public ImageSource {
public:

	/**
	 * Constructs a new directory image source.
	 *
	 * @param[in] directory The directory containing image files.
	 */
	explicit DirectoryImageSource(const string& directory);

	~DirectoryImageSource();

	void reset();

	bool next();

	const Mat getImage() const;

	path getName() const;

	vector<path> getNames() const;

private:
	vector<path> files; ///< The files of the given directory, ordered by name.
	int index;			///< The index of the next file.

};

} /* namespace imageio */
#endif /* DIRECTORYIMAGESOURCE_HPP_ */
