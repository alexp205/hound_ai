#include "hound_Win_err_parse.h"

std::wstring parseWinError(DWORD last_err_code);

DWORD last_err;

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