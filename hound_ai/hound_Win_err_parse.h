#pragma once

#ifndef _HOUND_WINDOWS_ERROR_PARSER_H_
#define _HOUND_WINDOWS_ERROR_PARSER_H_

#include <Windows.h>
#include <string>
#include <iostream>

std::wstring parseWinError(DWORD last_err_code);

#endif