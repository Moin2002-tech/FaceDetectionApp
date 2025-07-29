#pragma once

/*
* File: ImageSource.hpp
* Created on: 17/2/2025
* Author : Moin Shaikh
*/


#ifndef FEATUREDETECTION_IMAGEIO_IMAGESOURCE_HPP_
#define FEATUREDETECTION_IMAGEIO_IMAGESOURCE_HPP_
#include<opencv2/core.hpp>

#include<boost/filesystem/path.hpp>
#include<string>
#include<vector>


namespace imageio
{

	/**
	 * Source of subsequent images.
	*/
	class ImageSource
	{
	public:
		/**
		 * Constructs a new image source.
		 *
		 * @param[in] name The name of this image source.
		*/
		ImageSource(std::string name) :name(name) {}


		virtual ~ImageSource() {}

		/**
		 * Determines the name of this image source. Examples of possible names are the video file, image directory or device number.
		 *
		 * @return The name of this image source.
		 */
		const std::string& getSourceName() const 
		{
			return name;
		}

		/**
		 * Retrieves the next image. The result is the same as calling next(), followed by getImage().
		 *
		 * @return The image (that may be empty if no data could be retrieved).
		 */
		const cv::Mat get()
		{
			if (!next())
				return cv::Mat();
			return getImage();
		}

		/**
		 * Resets this source to its initial state.
		 */
		virtual void reset() = 0;

		/**
		 * Proceeds to the next image of this source.
		 *
		 * @return True if successful (so there was another image), false otherwise.
		 */
		virtual bool next() = 0;

		/**
		 * Retrieves the current image.
		 *
		 * @return The image (that may be empty if no data could be retrieved).
		 */
		virtual const cv::Mat getImage() const = 0;

		/**
		 * Retrieves the name of the current image. That could be the
		 * path to the image or the current frame number.
		 *
		 * @return The name of the current image (that may be empty if no data could be retrieved).
		 */
		virtual boost::filesystem::path getName() const = 0;	// Q: We could make this protected?

		/**
		 * Retrieves the list of image names currently in the image source.
		 *
		 * @return The image (that may be empty if no data could be retrieved)...
		 */
		virtual std::vector<boost::filesystem::path> getNames() const = 0;	// Q: We could make this protected?

	private:

		std::string name;
	};



}

#endif // !
