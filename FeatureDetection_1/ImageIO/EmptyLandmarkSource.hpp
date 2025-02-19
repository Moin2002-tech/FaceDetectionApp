/*
 * EmptyLandmarkSource.hpp
 *
 *  Created on: 17/2/2025
 *      Author: Moin Shaikh
 */

#ifndef FEATUREDETECTION_IMAGEIO_EMPTYLANDMARKSOURCE_HPP_
#define FEATUREDETECTION_IMAGEIO_EMPTYLANDMARKSOURCE_HPP_

#include "NamedLandmarkSource.hpp"
#include "LandmarkCollection.hpp"

namespace imageio {

/**
 * Landmark source without any landmarks.
 */
class EmptyLandmarkSource : public NamedLandmarkSource {
public:

	/**
	 * Constructs a new empty landmark source.
	 */
	EmptyLandmarkSource() : empty() {}

	void reset() {}

	bool next() {
		return true;
	}

	LandmarkCollection get(const boost::filesystem::path& imagePath) {
		return empty;
	}

	LandmarkCollection getLandmarks() const {
		return empty;
	}

	boost::filesystem::path getName() const {
		return boost::filesystem::path();
	}

private:

	const LandmarkCollection empty; ///< Empty landmark collection.
};

} /* namespace imageio */
#endif /* EMPTYLANDMARKSOURCE_HPP_ */
