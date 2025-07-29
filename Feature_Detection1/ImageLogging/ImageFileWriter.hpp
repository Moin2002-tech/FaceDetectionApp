
#pragma once

#ifndef FEATUREDETECTION_IMAGELOGGER_IMAGEFILEWRITER_HPP_
#define FEATUREDETECTION_IMAGELOGGER_IMAGEFILEWRITER_HPP_

#include "Appender.hpp"

#include "boost/filesystem/path.hpp"

using boost::filesystem::path;

namespace imagelogging {

/**
 * An appender that logs all messages equal or below its log-level to a text file.
 * The messages are appended to the log-file if the file already exists.
 */
class ImageFileWriter : public Appender {
public:

	/**
	 * Constructs a new appender that logs to a file. Appends to the file if it already exists.
	 *
	 * param[in] loglevel The loglevel at which to log.
	 * param[in] filename The full path to the file to log to.
	 */
	ImageFileWriter(loglevel logLevel, path directory);

	~ImageFileWriter();

	/**
	 * Appends a message to the opened file.
	 *
	 * @param[in] logLevel The log-level of the message.
	 * @param[in] loggerName The name of the logger that is logging the message.
	 * @param[in] logMessage The log-message itself.
	 */
	void log(const loglevel logLevel, const string loggerName, const string filename, Mat image, function<void ()> functionToApply, const string filenameSuffix);

private:

	path outputDirectory;

	/**
	 * Creates a new string containing the formatted current time.
	 *
	 * @return The formatted time.
	 */
	string getCurrentTime();

};

} /* namespace imagelogging */
#endif /* IMAGEFILEWRITER_HPP_ */
