/*
 * IbugLandmarkFormatParser.hpp
 *
 *  Created on: 17/2/2025
 *      Author: Moin Shaikh
 */
#pragma once

#ifndef FEATUREDETECTION_IMAGEIO_IBUGLANDMARKFORMATPARSER_HPP_
#define FEATUREDETECTION_IMAGEIO_IBUGLANDMARKFORMATPARSER_HPP_

#include "LandmarkFormatParser.hpp"
#include "LandmarkCollection.hpp"
#include "ModelLandmark.hpp"

#include "boost/filesystem.hpp"
#include <vector>
#include <string>
#include <map>
#include <memory>

namespace imageio {

/**
 * Takes landmarks in the form of .pts files from http://ibug.doc.ic.ac.uk/resources/300-W/ as
 * input and returns a map containing a LandmarkCollection in tlms format for every image.
 */
class IbugLandmarkFormatParser : public LandmarkFormatParser {
public:

	/**
	 * Reads the landmark data for one single image and returns all its landmarks.
	 *
	 * @param[in] landmarkFilePath A path to a .pts file.
	 * @return A map with one entry containing the basename of the file and
	 *         all 68 iBug landmarks.
	 */
	const std::map<boost::filesystem::path, LandmarkCollection> read(boost::filesystem::path landmarkFilePath);

};

} /* namespace imageio */
#endif /* IBUGLANDMARKFORMATPARSER_HPP_ */
