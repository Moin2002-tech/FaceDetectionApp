#pragma once
/*
* Appender.hpp
* Created on: 28/8/2024
* aurthor : Moin Shaikh
*/

#ifndef FEATUREDETECTION_LOGGING_LOGLEVEL_HPP_
#define FEATUREDETECTION_LOGGING_LOGLEVEL_HPP_


#include<string>
namespace logging {
	enum class LogLevel {
		Panic,	// Use to log extreme situations where the entire application execution could be affected due to some cause.
		Error,	// Use to log all errors and exceptions that affect the functionality of the application.
		Warn,	// Use to log warnings, i.e. situations that are usually unexpected but do not significantly affect the functionality.
		Info,	// Use to log statements which are expected to be written to the log during normal operations.
		Debug,	// Use to log messages helpful in debugging, but too verbose for normal situations.
		Trace	// Use to log method invocation or parameters
		/**
		 * Converts a LogLevel to a string.
		 *
		 * @param[in] logLevel The LogLevel that will be converted to a string.
		 * @return The LogLevel as a string.
		 */

	};
	static std::string logLevelToString(LogLevel logLevel) {

		switch (logLevel) {
		case LogLevel::Panic:
			return std::string("PANIC");
			break;

		case LogLevel::Error:
			return std::string("ERROR");
			break;

		case LogLevel::Warn:
			return std::string("WARN");
			break;
		case LogLevel::Info:
			return std::string("INFO");
			break;
		case LogLevel::Debug:
			return std::string("DEBUG");
			break;
		case LogLevel::Trace:
			return std::string("TRACE");
			break;
		}
	}





}


#endif // FEATUREDETECTION_LOGGING_LOGLEVEL_HPP_