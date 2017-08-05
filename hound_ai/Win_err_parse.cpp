#include "Win_err_parse.h"
#include <iostream>

std::wstring parseWinError(DWORD last_err_code);

DWORD last_err;

int main()
{
	last_err = GetLastError();

	std::wstring last_err_msg = parseWinError(last_err);
	std::wcout << L"Last detected error message: " << last_err_msg << std::endl;

	return 0;
}

std::wstring parseWinError(DWORD last_err_code)
{
	if (last_err_code == 0) return L"No error detected";

	LPWSTR msg_buf = nullptr;
	size_t size = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 
		NULL, 
		last_err_code, 
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
		(LPWSTR)&msg_buf, 
		0, 
		NULL);

	std::wstring msg(msg_buf, size);

	LocalFree(msg_buf);
	
	return msg;
}