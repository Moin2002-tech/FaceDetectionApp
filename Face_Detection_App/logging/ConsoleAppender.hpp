#pragma once
/*
* ConsoleAppender.hpp
* Created on: 28/8/2024
* aurthor : Moin Shaikh
*/

#ifndef FEATUREDETECTION_LOGGING_CONSOLEAPPENDER_HPP_
#define FEATUREDETECTION_LOGGING_CONSOLEAPPENDER_HPP_

#include"Appender.hpp"
#include"LogLevels.hpp"


namespace logging{
	class ConsoleAppender : public Appender {

	public:

		ConsoleAppender(LogLevel logLevel);
/**
	 * Constructs a new appender that logs to the console.
	 * TODO: This is the same as the one in our base class. Why do we
	 *			need to define this "again" here? Probably because if not,
	 *			the compiler generates a ConsoleAppender() default constructor?
	 *
	 * param[in] loglevel The LogLevel at which to log.
	 */

		/**
		  * Logs a message to the text console.
		  *
		  * @param[in] logLevel The log-level of the message.
		  * @param[in] loggerName The name of the logger that is logging the message.
		  * @param[in] logMessage The log-message itself.
		  */
	void log(const LogLevel logLevel, const std::string loggerName, std::string logMessage);

	private:

		/**
		 * Creates a new string containing the formatted current time.
		 *
		 * @return The formatted time.
		 */
		std::string getCurrentTime();
	};


}


#endif //FEATUREDETECTION_LOGGING_CONSOLEAPPENDER_HPP_