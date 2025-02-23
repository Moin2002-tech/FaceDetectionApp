/*
 * MuctLandmarkFormatParser.hpp
 *
 *  Created on: 17/2/2025
 *      Author: Moin Shaikh
 */
#pragma once

#ifndef FEATUREDETECTION_IMAGEIO_MUCTLANDMARKFORMATPARSER_HPP_
#define FEATUREDETECTION_IMAGEIO_MUCTLANDMARKFORMATPARSER_HPP_

#include "LandmarkFormatParser.hpp"
#include "LandmarkCollection.hpp"

#include "boost/filesystem/path.hpp"
#include <map>

namespace imageio {

/**
 * Reads landmarks from a CSV file from the MUCT database available at
 * https://code.google.com/p/muct/downloads/list.
 * Landmark id's/locations: http://www.milbo.org/muct/muct-landmarks.html
 * This parser reads the file where the landmarks are given in OpenCV-format.
 * In the MUCT distribution, the file is called 'muct76-opencv.csv'.
 *
 * Note the following excerpt from the MUCT readme about visibility:
 * "Unavailable points are marked with coordinates 0,0 (regardless of the
 * coordinate system mentioned above).  "Unavailable points" are points
 * that are obscured by other facial features.  (This refers to landmarks
 * behind the nose or side of the face -- the position of such landmarks
 * cannot easily be estimated by human landmarkers -- in contrast, the
 * position of landmarks behind hair or glasses was estimated by the
 * landmarkers).
 * So any points with the coordinates 0,0 should be ignored.  Unavailable
 * points appear only in camera views b and c.  Unless your software
 * knows how to deal with unavailable points, you should use only camera
 * views a, d, and e."
 */
class MuctLandmarkFormatParser : public LandmarkFormatParser {
public:

	/**
	 * Reads the landmark data for one single image and returns all its landmarks.
	 *
	 * @param[in] landmarkFilePath A path to a MUCT .csv file.
	 * @return A map with an entry for every image and every landmark.
	 */
	const std::map<boost::filesystem::path, LandmarkCollection> read(boost::filesystem::path landmarkFilePath);

};

} /* namespace imageio */
#endif /* MUCTLANDMARKFORMATPARSER_HPP_ */
