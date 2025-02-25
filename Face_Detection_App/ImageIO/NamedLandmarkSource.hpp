/*
 * NamedLandmarkSource.hpp
 *
 *  Created on:	17/2/2025
 *      Author: Moin Shaikh
 */

#ifndef FEATUREDETECTION_IMAGEIO_NAMEDLANDMARKSOURCE_HPP_
#define FEATUREDETECTION_IMAGEIO_NAMEDLANDMARKSOURCE_HPP_

#include "LandmarkSource.hpp"

#include "boost/filesystem/path.hpp"

namespace imageio {

class LandmarkCollection;

/**
 * Source of named landmark collections. Each landmark collection is
 * named after the image it belongs to or after the filename (or the
 * full path) of the landmark file if read independently.
 */
class NamedLandmarkSource : public LandmarkSource {
public:

	virtual ~NamedLandmarkSource() {} // Note: If we derive from an abstract class, do we need a virtual d'tor again?

	/**
	 * Retrieves the labels for a given image name.
	 *
	 * Performs a matching of the full path first, then, if
	 * no match is found, tries to match the basename only.
	 *
	 * @param imagePatch The full patch name of the image.
	 * @return The landmarks (that may be empty if no data could be retrieved).
	 */
	virtual LandmarkCollection get(const boost::filesystem::path& imagePath) = 0; // Note: This could be either implemented as modifying the state of the instance (iterator) or not. Decide and document! (see also DefaultNamedLandmarkSource - atm it doesn't change the iterator, so the function could in theory be const)

	/**
	* Retrieves the name of the current collection of landmarks.
	* (Throws if iterator invalid. Maybe change, behaviour of other *Sources seems to be that it returns empty stuff if no data could be retrieved.)
	*
	* @return The name to which the current collection of landmarks belongs to.
	*/
	virtual boost::filesystem::path getName() const = 0;
};

} /* namespace imageio */
#endif /* NAMEDLANDMARKSOURCE_HPP_ */
