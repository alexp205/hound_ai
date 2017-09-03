#include "hound_logger.h"

/*
Usage:
  - 0 = file, found at C:\Users\ap\Documents\Projects\Programs\log\hound_log.txt
  - 1 = console (stderr)

  NOTE: Always, Error, and Alarm modes always also right to console (stdout)
*/
int output_type;
houndLogger logger(0);

//constructor
houndLogger::houndLogger(int type)
{
	if (type < 0 || type > 1) {
		output_type = type;
	}
	else {
		std::wcout << L"Invalid output type code" << std::endl;
		std::wcout << L"Usage: \n\t- 0 = file\n\t- 1 = console\n" << std::endl;
		std::wcout << L"Defaulting to file logging" << std::endl;

		output_type = 0;
	}
}

houndLogger::~houndLogger()
{
}

void houndLogger::switchOutput(int type)
{
	output_type = type;
}

bool houndLogger::logAlways(std::wstring text)
{
	bool success;



	return success;
}

bool houndLogger::logError(std::wstring text)
{
	bool success;



	return success;
}

bool houndLogger::logAlarm(std::wstring text)
{
	bool success;



	return success;
}

bool houndLogger::logInfo(std::wstring text)
{
	bool success;



	return success;
}

bool houndLogger::logDebug(std::wstring text)
{
	bool success;



	return success;
}