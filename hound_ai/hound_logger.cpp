#include "hound_logger.h"

/*
Usage:
  - 0 = file, found at C:\Users\ap\Documents\Projects\Programs\log\hound_log.txt
  - 1 = console (stderr), essentially everything is output to console for debug purposes

  NOTE: Always, Error, and Alarm modes always also right to console (stdout)
*/
wchar_t log_file_path[256] = L"C:\\Users\\ap\\Documents\\Projects\\Programs\\logs\\";
int output_type;
std::wofstream log_file;

houndLogger logger(0);

//Constructor
houndLogger::houndLogger(int type)
{
	if (type >= 0 && type <= 1) {
		output_type = type;
	}
	else {
		std::wcout << L"Invalid output type code\n";
		std::wcout << L"Usage: \n\t- 0 = file\n\t- 1 = console\n\n";
		std::wcout << L"Defaulting to console logging" << std::endl;

		output_type = 1;
	}
}

//Destructor
houndLogger::~houndLogger()
{
	if (output_type == 0) log_file.close();
}

void houndLogger::initLogger()
{
	if (output_type == 0) {
		try {
			struct tm now;
			time_t cur = time(&cur);
			localtime_s(&now, &cur);
			std::wstring log_filename_str = L"hound_log_" + 
										int_to_wstring(now.tm_year + 1900) + L"-" + 
										int_to_wstring(now.tm_mon + 1) + L"-" +
										int_to_wstring(now.tm_mday) + L".txt";
			const wchar_t* log_filename = log_filename_str.c_str();
			wcscat_s(log_file_path, log_filename);
			log_file.open(log_file_path, std::ofstream::out | std::ofstream::app);
		}
		catch (const std::fstream::failure &e) {
			std::string exception_string = e.what();
			std::wcerr << get_exc_msg(exception_string) << std::endl;

			std::wcout << "Error opening file\n";
			std::wcout << "Defaulting to stderr output" << std::endl;
			output_type = 1;
		}
	}
}

void houndLogger::switchOutput(int type)
{
	output_type = type;
}


//Writes output to wcout and log file, no matter the current setting
bool houndLogger::logAlways(std::wstring text)
{
	bool success = (0 == 0);

	try {
		log_file.write((char*)&text[0], text.size());
	}
	catch (const std::fstream::failure &e) {
		std::string exception_string = e.what();
		std::wcerr << get_exc_msg(exception_string) << std::endl;

		success = (-1 == 0);
	}

	std::wcout << text << std::endl;

	return success;
}

/*
Writes output to wcerr or log file, depending on the current setting

This is the lowest level of error
*/
bool houndLogger::logError(std::wstring text)
{
	bool success = (0 == 0);
	std::wstring out_text = L"----" + text;

	if (output_type == 0) {
		try {
			log_file.write((char*)&out_text[0], out_text.size());
		}
		catch (const std::fstream::failure &e) {
			std::string exception_string = e.what();
			std::wcerr << get_exc_msg(exception_string) << std::endl;

			std::wcerr << "Error writing to file\n";
			std::wcerr << "Dumping to stderr output" << std::endl;
			std::wcerr << text << std::endl;
			success = (-1 == 0);
		}
	}
	else {
		std::wcerr << text << std::endl;
	}

	return success;
}

/*
Writes output to wcerr and log file, no matter the current setting

This is the highest level of error
*/
bool houndLogger::logAlarm(std::wstring text)
{
	bool success = (0 == 0);

	std::wstring out_text = L"--------" + text;

	try {
		log_file.write((char*)&out_text[0], out_text.size());
	}
	catch (const std::fstream::failure &e) {
		std::string exception_string = e.what();
		std::wcerr << get_exc_msg(exception_string) << std::endl;

		std::wcerr << "Error writing to file\n";
		std::wcerr << "Dumping to stderr output" << std::endl;
		std::wcerr << text << std::endl;
		success = (-1 == 0);
	}

	std::wcerr << text << std::endl;

	return success;
}

//Writes output to wcout or log file, depending on the current setting
bool houndLogger::logInfo(std::wstring text)
{
	bool success = (0 == 0);

	if (output_type == 0) {
		try {
			log_file.write((char*)&text[0], text.size());
		}
		catch (const std::fstream::failure &e) {
			std::string exception_string = e.what();
			std::wcerr << get_exc_msg(exception_string) << std::endl;

			success = (-1 == 0);
		}
	}
	else {
		std::wcout << text << std::endl;
	}

	return success;
}

//Writes output to wcout
bool houndLogger::logDebug(std::wstring text)
{
	bool success = (0 == 0);

	std::wcout << text << std::endl;

	return success;
}

std::wstring houndLogger::get_exc_msg(std::string raw_exc_msg)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring exception_msg = converter.from_bytes(raw_exc_msg);
	std::wstring log_msg = L"Standard exception: " + exception_msg;

	return log_msg;
}

std::wstring houndLogger::int_to_wstring(int input)
{
	std::wostringstream ss;
	ss << input;
	std::wstring out(ss.str());
	return out;
}