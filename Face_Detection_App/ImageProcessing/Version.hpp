#pragma once
/*
* File: Version.hpp
* Created on: 4/2/2025
* Author : Moin Shaikh
*/

#ifndef FEATUREDETECTION_IMAGEPROCESSING_VERSION_HPP_
#define FEATUREDETECTION_IMAGEPROCESSING_VERSION_HPP_


#include<ostream>

namespace ImageProcessing 
{
	/**
		* Version number.
	*/
	class Version
	{
	public:

		Version() : instance(nextInstance++), version(0) 
		{

		}
		Version(const Version& other) = default;

		Version& operator++() 
		{
			++version;
			return *this;
		}

		bool operator==(const Version& other) const 
		{
			return instance == other.instance && version == other.version;
		}

		bool operator!=(const Version& other) const
		{
			return !(*this == other);
		}

		friend std::ostream& operator<<(std::ostream& out, const Version& version);

	private:
		static int nextInstance;

		int instance;
		int version;
	};

}

#endif //FEATUREDETECTION_IMAGEPROCESSING_VERSION_HPP_