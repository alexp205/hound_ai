#pragma once

#ifndef HOUND_DLL_INJECTOR_H_
#define HOUND_DLL_INJECTOR_H_

#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include "hound_logger.h"

void setupInject(wchar_t* DLL_file, wchar_t* proc, DWORD proc_id, PROCESSENTRY32W pe32, HANDLE hproc_snapshot);
bool InjectDLL(wchar_t* DLL_file, DWORD proc_id);

extern houndLogger logger;
extern std::wstring log_msg;

#endif