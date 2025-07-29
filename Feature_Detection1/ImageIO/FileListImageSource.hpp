/*
 * FileListImageSource.hpp
 *
 *  Created on: 17/2/2025
 *      Author: Moin Shaikh
 */

#ifndef FEATUREDETECTION_IMAGEIO_FILELISTIMAGESOURCE_HPP_
#define FEATUREDETECTION_IMAGEIO_FILELISTIMAGESOURCE_HPP_

#include "ImageSource.hpp"

#include "boost/filesystem.hpp"
#include "opencv2/core/core.hpp"
#include <vector>

using boost::filesystem::path;
using std::vector;
using std::string;
using cv::Mat;

namespace imageio {

/**
 * Image source that reads a text file containing an image filename on each line
 * and creates an image source from it.
 */
class FileListImageSource : public ImageSource {
public:

	/**
	 * Constructs a new file-list image source.
	 *
	 * @param[in] filelist A text-file containing a list of files.
	 * @param[in] pathPrefix A path that is added in front of the image name found in the list-file.
	 * @param[in] alternativeExtension This optional extension is added or the current one replaced by this one.
	 */
	explicit FileListImageSource(const string& filelist, const string& pathPrefix="", const string& alternativeExtension="");

	~FileListImageSource();

	void reset();

	bool next();

	const Mat getImage() const;

	path getName() const;

	vector<path> getNames() const;

private:
	vector<path> files;	///< The files of the given directory, ordered by name. TODO: No, I think they're not ordered. Check the code! Anyway I think expected behaviour is that they should be in the same order as in the list-file.
	int index;			///< The index of the next file.
};

} /* namespace imageio */
#endif /* FILELISTIMAGESOURCE_HPP_ */
