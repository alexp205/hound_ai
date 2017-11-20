#include "hound_logger.h"

/*
Usage:
  - 0 = file, found at C:\Users\ap\Documents\Projects\Programs\log\hound_log.txt
  - 1 = console (stderr), essentially everything is output to console for debug purposes

  NOTE: Always, Error, and Alarm modes always also right to console (stdout)
*/
std::wstring log_file_path = L"C:\\Users\\ap\\Documents\\Projects\\logs\\";
int output_type;
char time_info[64];

houndLogger logger(0);
std::wstring log_msg(L"");

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
			log_file_path = log_file_path + log_filename_str;
		}
		catch (const std::fstream::failure &e) {
			std::string exception_string = e.what();
			exception_string = "Standard exception: " + exception_string;
			std::wcerr << norm_to_wide(exception_string) << std::endl;

			std::wcout << L"Error opening file\n";
			std::wcout << L"Defaulting to stderr output" << std::endl;
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
		std::wofstream log_file;
		log_file.open(log_file_path, std::ofstream::out | std::ofstream::app);

		time_t cur = time(&cur);
		ctime_s(time_info, sizeof(time_info), &cur);
		std::string time_text(time_info);
		log_file << norm_to_wide(time_text);
		log_file << text << L"\n" << L"-------------------------------------------------------------------------" << std::endl;
		memset(time_info, 0, sizeof(time_info));
	}
	catch (const std::fstream::failure &e) {
		std::string exception_string = e.what();
		exception_string = "Standard exception: " + exception_string;
		std::wcerr << norm_to_wide(exception_string) << std::endl;

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
	std::wstring out_text = L"ERROR ---- " + text + L"\n";

	if (output_type == 0) {
		try {
			std::wofstream log_file;
			log_file.open(log_file_path, std::ofstream::out | std::ofstream::app);

			time_t cur = time(&cur);
			ctime_s(time_info, sizeof(time_info), &cur);
			std::string time_text(time_info);
			log_file << norm_to_wide(time_text);
			log_file << out_text << L"-------------------------------------------------------------------------" << std::endl;
			memset(time_info, 0, sizeof(time_info));
		}
		catch (const std::fstream::failure &e) {
			std::string exception_string = e.what();
			exception_string = "Standard exception: " + exception_string;
			std::wcerr << norm_to_wide(exception_string) << std::endl;

			std::wcerr << L"Error writing to file\n";
			std::wcerr << L"Dumping to stderr output" << std::endl;
			std::wcerr << out_text << std::endl;
			success = (-1 == 0);
		}
	}
	else {
		std::wcerr << out_text << std::endl;
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

	std::wstring out_text = L"ALARM -------- " + text + L"\n";

	try {
		std::wofstream log_file;
		log_file.open(log_file_path, std::ofstream::out | std::ofstream::app);

		time_t cur = time(&cur);
		ctime_s(time_info, sizeof(time_info), &cur);
		std::string time_text(time_info);
		log_file << norm_to_wide(time_text);
		log_file << out_text << L"-------------------------------------------------------------------------" << std::endl;
		memset(time_info, 0, sizeof(time_info));
	}
	catch (const std::fstream::failure &e) {
		std::string exception_string = e.what();
		exception_string = "Standard exception: " + exception_string;
		std::wcerr << norm_to_wide(exception_string) << std::endl;

		std::wcerr << L"Error writing to file\n";
		std::wcerr << L"Dumping to stderr output" << std::endl;
		std::wcerr << out_text << std::endl;
		success = (-1 == 0);
	}

	std::wcerr << out_text << std::endl;

	return success;
}

//Writes output to wcout or log file, depending on the current setting
bool houndLogger::logInfo(std::wstring text)
{
	bool success = (0 == 0);

	if (output_type == 0) {
		try {
			std::wofstream log_file;
			log_file.open(log_file_path, std::ofstream::out | std::ofstream::app);

			time_t cur = time(&cur);
			ctime_s(time_info, sizeof(time_info), &cur);
			std::string time_text(time_info);
			log_file << norm_to_wide(time_text);
			log_file << text << L"\n" << L"-------------------------------------------------------------------------" << std::endl;
			memset(time_info, 0, sizeof(time_info));
		}
		catch (const std::fstream::failure &e) {
			std::string exception_string = e.what();
			exception_string = "Standard exception: " + exception_string;
			std::wcerr << norm_to_wide(exception_string) << std::endl;

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

std::wstring houndLogger::norm_to_wide(std::string norm_msg)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring wide_msg = converter.from_bytes(norm_msg);
	log_msg = wide_msg + L"\n";

	return log_msg;
}

std::wstring houndLogger::int_to_wstring(int input)
{
	std::wostringstream ss;
	ss << input;
	std::wstring out(ss.str());
	return out;
}