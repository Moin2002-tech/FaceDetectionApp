/*
 * LandmarkFormatParser.hpp
 *
 *  Created on: 17/2/2025
 *      Author: Moin Shaikh
 */

#ifndef FEATUREDETECTION_IMAGEIO_LANDMARKFORMATPARSER_HPP_
#define FEATUREDETECTION_IMAGEIO_LANDMARKFORMATPARSER_HPP_

#include "LandmarkCollection.hpp"
#include "boost/filesystem.hpp"
#include <map>
#include <string>

namespace imageio {

/**
 * Takes a path to a landmarks file as input and returns a map with one
 * LandmarkCollection entry for each image with all the landmarks found.
 * The landmark files stored are identified only by their basename (because
 * landmark files could be stored in another folder and have a different
 * file extension than the image).
 */
class LandmarkFormatParser {
public:

	virtual ~LandmarkFormatParser() {}

	/**
	 * Reads the landmark data from a file returns all its landmarks.
	 *
	 * @param[in] landmarkFilePath A path to a file containing landmarks to one or several images/frames.
	 * @return All the landmarks that are present in the input. The path is
	 *         stripped to only contain the basename.
	 */
	virtual const std::map<boost::filesystem::path, LandmarkCollection> read(boost::filesystem::path landmarkFilePath) = 0;
};

} /* namespace imageio */
#endif /* LANDMARKFORMATPARSER_HPP_ */
