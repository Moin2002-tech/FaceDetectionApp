/*
* Logger.hpp
* Created on : 28 / 8 / 2024
* aurthor : Moin Shaikh
*/

#include"Logger.hpp"
#include"Appender.hpp"

using std::string;
using std::shared_ptr;

namespace logging{
	Logger::Logger(string name) : name(name), appenders() {  };

	void Logger::log(const LogLevel logLevel, string logMessage) {
		for (shared_ptr<Appender> appender : appenders) {
			appender->log(logLevel, name, logMessage);
		}
	}
	void Logger::addAppender(shared_ptr<Appender> appender) {
		appenders.push_back(appender);
	}



	void Logger::trace(const string logMessage)
	{
		log(LogLevel::Trace, logMessage);
	}

	void Logger::debug(const string logMessage)
	{
		log(LogLevel::Debug, logMessage);
	}

	void Logger::info(const string logMessage)
	{
		log(LogLevel::Info, logMessage);
	}

	void Logger::warn(const string logMessage)
	{
		log(LogLevel::Warn, logMessage);
	}

	void Logger::error(const string logMessage)
	{
		log(LogLevel::Error, logMessage);
	}

	void Logger::panic(const string logMessage)
	{
		log(LogLevel::Panic, logMessage);
	}





}
