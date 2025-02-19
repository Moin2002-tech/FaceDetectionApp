
#pragma once
/*
* Appender.hpp
* Created on : 28 / 8 / 2024
* aurthor : Moin Shaikh
*/
#include<fstream>
#include<string>
#include<vector>
#include<memory>
#include"Appender.hpp"

#ifndef FEATUREDETECTION_LOGGING_LOGGER_HPP_
#define FEATUREDETECTION_LOGGING_LOGGER_HPP_

namespace logging {
	

	class Logger {
	public:

		explicit Logger(std::string name);
		void addAppender(std::shared_ptr<Appender> appender);
		void trace(const std::string logMessage);
		void debug(const std::string logMessage);
		void info(const std::string logMessage);
		void warn(const std::string logMessage);
		void error(const std::string logMessage);
		void panic(const std::string logMessage);
		
	private:
		std::string name;
		std::vector<std::shared_ptr<Appender>> appenders;

		/**
		 * Logs a message to all appenders (e.g. the console or a file) with corresponding log-levels.
		 *
		 * @param[in] logLevel The log-level of the message.
		 * @param[in] logMessage The message to be logged.
		 */
		void log(const LogLevel logLevel, const std::string logMessage);

	};
}
#endif //FEATUREDETECTION_LOGGING_LOGGER_HPP_