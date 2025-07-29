/*
 * SimpleModelLandmarkSink.hpp
 *
 *  Created on: 17/2/2025
 *      Author: Moin Shaikh
 */
#pragma once

#ifndef FEATUREDETECTION_IMAGEIO_SIMPLEMODELLANDMARKSINK_HPP_
#define FEATUREDETECTION_IMAGEIO_SIMPLEMODELLANDMARKSINK_HPP_

#include "NamedLandmarkSink.hpp"


#include "boost/filesystem/path.hpp"

namespace imageio {

class LandmarkCollection;

/**
 * Sink for landmark collections where each collection
 * is saved to a separate file.
 * Each line (i.e. each landmark) in a file is written
 * as 'name x y'.
 */
class SimpleModelLandmarkSink : public NamedLandmarkSink {
public:

	/**
	 * Adds a landmark collection and saves it to the given file.
	 *
	 * @param[in] collection The landmark collection.
	 * @param[in] filename The file to which to save the landmarks, including file extension.
	 */
	void add(const LandmarkCollection& collection, boost::filesystem::path filename);
};

} /* namespace imageio */
#endif /* SIMPLEMODELLANDMARKSINK_HPP_ */
