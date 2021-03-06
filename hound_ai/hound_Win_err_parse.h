#pragma once

#ifndef HOUND_WINDOWS_ERROR_PARSER_H_
#define HOUND_WINDOWS_ERROR_PARSER_H_

#include <Windows.h>
#include <string>
#include <iostream>
#include "hound_logger.h"

std::wstring parseWinError(DWORD last_err_code);

extern houndLogger logger;

#endif