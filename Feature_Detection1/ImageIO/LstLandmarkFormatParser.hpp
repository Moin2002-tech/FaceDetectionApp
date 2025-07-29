/*
 * LstLandmarkFormatParser.hpp
 *
 *  Created on: 17/2/2025
 *      Author: Moin Shaikh
 */
#pragma once

#ifndef FEATUREDETECTION_IMAGEIO_LSTLANDMARKFORMATPARSER_HPP_
#define FEATUREDETECTION_IMAGEIO_LSTLANDMARKFORMATPARSER_HPP_

#include "LandmarkFormatParser.hpp"
#include "LandmarkCollection.hpp"

#include "boost/filesystem.hpp"
#include <map>

namespace imageio {

/**
 * Reads landmarks in the form of one .lst file that contains one line for each image.
 * A line consists of the path to the image, followed by 'x_topleft y_topleft x_bottomright y_bottomright'
 * of the face box (as integers).
 */
class LstLandmarkFormatParser : public LandmarkFormatParser {
public:

	/**
	 * Reads the landmark data for one single image and returns all its landmarks (TODO in tlms format).
	 *
	 * @param[in] landmarkFilePath A path to a .pts file.
	 * @return A map with one entry containing the basename of the file and
	 *         all the landmarks that are present (TODO in tlms format).
	 */
	const std::map<boost::filesystem::path, LandmarkCollection> read(boost::filesystem::path landmarkFilePath);

};

} /* namespace imageio */
#endif /* LSTLANDMARKFORMATPARSER_HPP_ */
