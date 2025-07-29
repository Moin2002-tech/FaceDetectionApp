/*
 * RectLandmarkSink.hpp
 *
 *  Created on:	17/2/2025
 *      Author: Moin Shaikh
 */
#pragma once

#ifndef FEATUREDETECTION_IMAGEIO_RECTLANDMARKSINK_HPP_
#define FEATUREDETECTION_IMAGEIO_RECTLANDMARKSINK_HPP_


#include "boost/filesystem/path.hpp"

namespace imageio {

class LandmarkCollection;

/**
 * Landmark sink that writes rectangular landmarks to a
 * file using a format of one line per landmark in the form 
 * of 'landmarkName topLeftX topLeftY width height'.
 * Generates one landmark file for each image (i.e. for
 * each call of the 'write' method).
 *
 */
class RectLandmarkSink { // TODO: We should probably derive from Ordered/NamedLandmarkSink. Also this seems to be the same as SimpleLandmarkSink?
public:

	/**
	 * Constructs a new default landmark sink.
	 *
	 * @param[in] outputPath A path to the folder to which the landmark files should be written.
	 */
	explicit RectLandmarkSink(const boost::filesystem::path& outputPath);

	/**
	 * Creates a file and writes the landmarks to it.
	 *
	 * @param[in] collection All the rect landmarks to write to the file.
	 * @param[in] imageFilename The filename of the image. Only the basename will be used.
	 */
	void write(const LandmarkCollection& collection, const boost::filesystem::path imageFilename);

private:

	const boost::filesystem::path outputDirectory; ///< The folder to which the landmark files should be saved.

};

} /* namespace imageio */
#endif /* RECTLANDMARKSINK_HPP_ */
