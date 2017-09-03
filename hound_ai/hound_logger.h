#pragma once

#ifndef _HOUND_LOGGER_H_
#define _HOUND_LOGGER_H_

#include <iostream>

class houndLogger {
	int output_type;

public:
	houndLogger(int type);
	void switchOutput(int type);
	bool logAlways(std::wstring text);
	bool logError(std::wstring text);
	bool logAlarm(std::wstring text);
	bool logInfo(std::wstring text);
	bool logDebug(std::wstring text);
};

static houndLogger logger;

#endif