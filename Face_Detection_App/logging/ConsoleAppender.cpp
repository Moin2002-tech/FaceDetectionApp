/*
* ConsoleAppender.cpp
* Created on: 28/8/2024
* aurthor : Moin Shaikh
*/
#define _CRT_SECURE_NO_WARNINGS

#include"ConsoleAppender.hpp"
#include"LogLevels.hpp"
#include<iostream>
#include<iomanip>
#include<sstream>
#include<chrono>
#include<cstdint>


using std::cout;
using std::string;
using std::ostringstream;
using std::chrono::system_clock;
using std::chrono::duration;
using std::chrono::duration_cast;

namespace logging {
	ConsoleAppender::ConsoleAppender(LogLevel loglevel) : Appender(loglevel) {};


	void ConsoleAppender::log(const LogLevel logLevel, const string loggerName, const string logMessage) {
		if (logLevel <= this->logLevel) {
			cout << getCurrentTime() << ' ' << logLevelToString(logLevel) << ' ' << "[" << loggerName << "] " << logMessage << std::endl;
		}
	}

	string ConsoleAppender::getCurrentTime() {
		system_clock::time_point now = system_clock::now();
		duration<int64_t, std::ratio<1>> seconds = duration_cast<duration<int64_t, std::ratio<1>>>(now.time_since_epoch());
		duration<int64_t, std::milli> miliSeconds = duration_cast<duration<int64_t, std::milli>>(now.time_since_epoch());
		duration<int64_t, std::milli> microSeconds = miliSeconds - seconds;
		std::time_t time_now = system_clock::to_time_t(now);
		struct tm* tm_no = localtime(&time_now);
		ostringstream os;
		os.fill('0');
		os << std::setw(2) << tm_no->tm_hour << ':' << std::setw(2) << tm_no->tm_min << ':' << std::setw(2) << tm_no->tm_sec;
		os << '.' << std::setw(3) << microSeconds.count();
		return os.str();


	}



}
