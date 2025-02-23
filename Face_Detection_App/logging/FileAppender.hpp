#pragma once
/*
* Appender.hpp
* Created on : 28 / 8 / 2024
* aurthor : Moin Shaikh


*/

#ifndef FEATUREDETECTION_LOGGING_FILEAPPENDER_HPP_
#define FEATUREDETECTION_LOGGING_FILEAPPENDER_HPP_
#include "Appender.hpp"
#include<fstream>
namespace logging {
class FileAppender : public Appender {
public:

	FileAppender(LogLevel logLevel, std::string fileName);

	~FileAppender();

	void log(const LogLevel loglevel, const std::string loggerName, std::string logMessage);



private:

	/**
	 * Creates a new string containing the formatted current time.
	 *
	 * @return The formatted time.
	 */
	std::string getCurrentTime();

	// TODO: We should make the copy constructor (and assignment operator?) private because we have an ofstream as member variable! Read that somewhere on stackoverflow.
	std::ofstream file;




};
}

#endif // !FEATUREDETECTION_LOGGING_FILEAPPENDER_HPP_


