#pragma once

#ifndef _WINDOWS_ERROR_PARSER_H_
#define _WINDOWS_ERROR_PARSER_H_

#include <Windows.h>
#include <string>

std::wstring parseWinError(DWORD last_err_code);

#endif