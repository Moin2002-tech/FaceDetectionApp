/*
 * PascStillEyesLandmarkFormatParser.hpp
 *
 *  Created on: 17/2/2025
 *      Author: Moin Shaikh
 */
#pragma once

#ifndef FEATUREDETECTION_IMAGEIO_PASCSTILLEYESLANDMARKFORMATPARSER_HPP_
#define FEATUREDETECTION_IMAGEIO_PASCSTILLEYESLANDMARKFORMATPARSER_HPP_

#include "LandmarkFormatParser.hpp"
#include "LandmarkCollection.hpp"

#include "boost/filesystem/path.hpp"
#include <map>

namespace imageio {

/**
 * Reads landmarks from the CSV file 'pasc_still_pittpatt_eye_coor.csv'
 * from the Point and Shoot Database (http://www.nist.gov/itl/iad/ig/pasc.cfm).
 *
 * The file contains eye landmarks from the commercial PittPatt
 * face detection software.
 *
 * Convention from PaSC:
 * The convention is that left eye means left side of the image as presented.
 * The origin of all images is the top left corner - x horizontal y vertical (down).
 * Pixel zero zero is the top left corner.
 */
class PascStillEyesLandmarkFormatParser : public LandmarkFormatParser {
public:

	/**
	 * Reads the landmark data for one single image and returns all its landmarks.
	 *
	 * @param[in] landmarkFilePath A path to the PaSC .csv file.
	 * @return A map with an eye-landmark entry for every image.
	 */
	const std::map<boost::filesystem::path, LandmarkCollection> read(boost::filesystem::path landmarkFilePath);

};

} /* namespace imageio */
#endif /* PASCSTILLEYESLANDMARKFORMATPARSER_HPP_ */
