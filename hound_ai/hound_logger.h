#pragma once

#ifndef HOUND_LOGGER_H_
#define HOUND_LOGGER_H_

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <codecvt>
#include <ctime>

class houndLogger {

	int output_type;
	std::ofstream log_file;

public:
	houndLogger(int type);
	~houndLogger();
	void initLogger();
	void switchOutput(int type);
	bool logAlways(std::wstring text);
	bool logError(std::wstring text);
	bool logAlarm(std::wstring text);
	bool logInfo(std::wstring text);
	bool logDebug(std::wstring text);
	std::wstring norm_to_wide(std::string raw_exc_msg);
	std::wstring int_to_wstring(int input);
};

extern houndLogger logger;

#endif