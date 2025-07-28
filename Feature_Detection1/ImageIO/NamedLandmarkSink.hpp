/*
 * NamedLandmarkSink.hpp
 *
 *  Created on: 17/2/2025
 *      Author: Moin Shaikh
 */
#pragma once

#ifndef FEATUREDETECTION_IMAGEIO_NAMEDLANDMARKSINK_HPP_
#define FEATUREDETECTION_IMAGEIO_NAMEDLANDMARKSINK_HPP_


#include "boost/filesystem/path.hpp"

namespace imageio {

class LandmarkCollection;

/**
 * Sink for landmark collections where each landmark
 * is saved to a separate file with a filename.
 */
class NamedLandmarkSink {
public:

	virtual ~NamedLandmarkSink() {}

	/**
	 * Adds a landmark collection and saves it to the given file.
	 *
	 * @param[in] collection The landmark collection.
	 * @param[in] filename The file to which to save the landmarks, without file extension.
	 */
	virtual void add(const LandmarkCollection& collection, boost::filesystem::path filename) = 0;
};

} /* namespace imageio */
#endif /* NAMEDLANDMARKSINK_HPP_ */
