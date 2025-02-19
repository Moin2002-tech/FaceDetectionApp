/*
* loggerFactory.hpp
* Created on : 28 / 8 / 2024
* aurthor : Moin Shaikh
*/

#include"LoggerFactory.hpp"
#include<utility>
#include<sstream>

using std::map;
using std::pair;
using std::string;
using std::ostringstream;
namespace logging 
	{
	LoggerFactory::LoggerFactory() 
	{

	}
	LoggerFactory::~LoggerFactory() 
	{

	}


	LoggerFactory* LoggerFactory::Instance()
	{
		static LoggerFactory instance;
		return &instance;
	}

	Logger& LoggerFactory::getLogger(string name)
	{
		map<string, Logger>::iterator it = loggers.find(name);
		if (it != loggers.end())
		{
			return it->second;
		}
		else
		{
			Logger logger = Logger(name);
			pair<map<string, Logger>::iterator, bool>  ret = loggers.insert(make_pair(name, logger));
			if (ret.second == false)
			{

			}
			return ret.first->second;
		}
		
	}

	Logger& LoggerFactory::getLoggerFor(const string fileName) {
		int lastSlash = fileName.find_last_of("/\\");
		int lastDot = fileName.find_last_of(".");
		return getLogger(fileName.substr(lastSlash + 1, lastDot - lastSlash - 1));
	}

}