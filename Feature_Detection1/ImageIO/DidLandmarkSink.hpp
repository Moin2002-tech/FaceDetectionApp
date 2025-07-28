/*
 * DidLandmarkSink.hpp
 *
 *  Created on: 17/2/2025
 *      Author: Moin Shaikh
 */
#pragma once

#ifndef FEATUREDETECTION_IMAGEIO_DIDLANDMARKSINK_HPP_
#define FEATUREDETECTION_IMAGEIO_DIDLANDMARKSINK_HPP_

#include "NamedLandmarkSink.hpp"


#include "boost/filesystem/path.hpp"

namespace imageio {

class LandmarkCollection;

/**
 * Sink for landmark collections where each landmark
 * is saved to a separate file in the Surrey DID
 * format. Each line is 'x y vertexId'.
 * TODO/Note: x and y are ints? What about float? CHECK!
 */
class DidLandmarkSink : public NamedLandmarkSink {
public:

	/**
	 * Adds a landmark collection and saves it to the given file.
	 *
	 * @param[in] collection The landmark collection.
	 * @param[in] filename The file to which to save the landmarks. The current file extension will be replaced by '.did', or, if none given, it will be added.
	 */
	void add(const LandmarkCollection& collection, boost::filesystem::path filename);
};

} /* namespace imageio */
#endif /* DIDLANDMARKSINK_HPP_ */
