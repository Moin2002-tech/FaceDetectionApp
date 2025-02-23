/*
* File: Version.cpp
* Created on: 11/2/2025
* Author : Moin Shaikh
*/

#include"Version.hpp"

namespace ImageProcessing
{
	int Version::nextInstance = 0;

	std::ostream& operator<<(std::ostream& out, const Version &version)
	{
		return out << version.instance << ":" << version.version;
	}
}
