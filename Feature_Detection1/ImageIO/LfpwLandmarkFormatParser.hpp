/*
 * LfpwLandmarkFormatParser.hpp
 *
 *  Created on: 17/2/2025
 *      Author: Moin Shaikh
 */
#pragma once

#ifndef FEATUREDETECTION_IMAGEIO_LFPWLANDMARKFORMATPARSER_HPP_
#define FEATUREDETECTION_IMAGEIO_LFPWLANDMARKFORMATPARSER_HPP_

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
 * Takes the path to a .csv file from http://homes.cs.washington.edu/~neeraj/databases/lfpw/ as
 * input and returns a map containing a LandmarkCollection in tlms format for every image in
 * the .csv file. The 'average' landmark position (out of the 3 MTurkers) is read from the file.
 */
class LfpwLandmarkFormatParser : public LandmarkFormatParser {
public:

	~LfpwLandmarkFormatParser();

	/**
	 * Reads the landmark data for one single image and returns all its landmarks (TODO in tlms format).
	 *
	 * @param[in] landmarkFilePath A path to a .did file.
	 * @return A map with one entry containing the basename of the file and
	 *         all the landmarks that are present (TODO in tlms format).
	 */
	const std::map<boost::filesystem::path, LandmarkCollection> read(boost::filesystem::path landmarkFilePath);

private:
	static std::map<std::string, std::string> lfpwLmMapping;	///< Contains a mapping from the LFPW landmark names to tlms landmark names.
	
	/**
	 * Parse a line of a .did file and return a Landmark. TODO
	 *
	 * @param[in] line The line with the landmark information to parse, as tokens pre-split at '\t'.
	 * @param[in] header TODO.
	 * @return TODO.
	 */
	std::pair<boost::filesystem::path, LandmarkCollection> readLine(const std::vector<std::string>& line, const std::string header);
	
public:
	static std::string lfpwToTlmsName(std::string lfpwName);
};

} /* namespace imageio */
#endif /* LFPWLANDMARKFORMATPARSER_HPP_ */
