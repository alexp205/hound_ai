#include "hound_logger.h"

/*
Usage:
  - 0 = file, found at C:\Users\ap\Documents\Projects\Programs\log\hound_log.txt
  - 1 = console (stderr)

  NOTE: Always, Error, and Alarm modes always also right to console (stdout)
*/
wchar_t log_file_path[256] = L"C:\\Users\\ap\\Documents\\Projects\\Programs\\log\\hound_log.txt";
int output_type;
std::ofstream log_file;

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
	log_file.close();
}

void houndLogger::initLogger()
{
	if (output_type == 0) {
		try {
			std::streampos begin, end;
			std::fstream check_file(log_file_path, std::ios::binary);
			begin = check_file.tellg();
			check_file.seekg(0, std::ios::end);
			end = check_file.tellg();
			check_file.close();
			std::streampos file_size = (end - begin);
			if (file_size > 10 * 1024 * 1024) {

				_wrename(log_file_path, );
			}

			log_file.open(log_file_path);

		}
		catch (const std::fstream::failure& e) {

		}
	}
}

void houndLogger::switchOutput(int type)
{
	output_type = type;
}

bool houndLogger::swapLog()
{

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