#pragma once


/*
* Appender.hpp
* Created on: 28/8/2024
* aurthor : Moin Shaikh
*/

#ifndef FEATUREDETECTION_LOGGING_LOGGERFACTORY_HPP_
#define FEATUREDETECTION_LOGGING_LOGGERFACTORY_HPP_
#include"ConsoleAppender.hpp"
#include"LogLevels.hpp"
#include"Logger.hpp"

#include<map>
#include<string>
#include<memory>


namespace logging {



	class LoggerFactory {
	private:
		LoggerFactory();
		~LoggerFactory();
		LoggerFactory(const LoggerFactory&);
		LoggerFactory& operator = (const LoggerFactory&);



	public:
		static LoggerFactory* Instance();

		Logger& getLogger(const std::string name);
		Logger& getLoggerFor(const std::string fileName);

		void load(std::string fileName) {};
		
	private:
		
		std::map < std::string, Logger > loggers;

	};
}

#endif //FEATUREDETECTION_LOGGING_LOGGERFACTORY_HPP_