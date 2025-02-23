#pragma once
/*
* File: VersionedImage.hpp
* Created on: 4/2/2025
* Author : Moin Shaikh
*/


#ifndef FEATUREDETECTION_IMAGEPROCESSING_VERSIONEDIMAGE_HPP_
#define FEATUREDETECTION_IMAGEPROCESSING_VERSIONEDIMAGE_HPP_


#include"Version.hpp"
#include<opencv2/core.hpp>
using cv::Mat;


namespace ImageProcessing
{
	/**
	 * Image with a version number.
	*/
	class VersionedImage
	{
	public:

		/**
		* Constructs a new empty versioned image.
		*/	
		VersionedImage() : data(), version() {}

		/**
		 * Constructs a new versioned image.
		 *
		 * @param[in] data The image data.
		*/
		explicit VersionedImage(const Mat& data) : data(data), version()
		{

		}

		/**
			* @return The image data.
		*/
		cv::Mat& getData() 
		{
			return data;
		}

		/**
		 * @return The image data.
		 */
		const cv::Mat& getData() const 
		{
			return data;
		}

		/**
		 * @param[in] data The new image data.
		 */
		void setData(const cv::Mat& data)
		{
			this->data = data;
			++version;
		}

		/**
		 * @return The version.
		 */
		Version getVersion() const 
		{
			return version;
		}

	private:
		Mat data;
		Version version;
	};

	
}


#endif //FEATUREDETECTION_IMAGEPROCESSING_VERSIONEDIMAGE_HPP_
