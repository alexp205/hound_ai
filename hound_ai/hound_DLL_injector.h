#pragma once

#ifndef HOUND_DLL_INJECTOR_H_
#define HOUND_DLL_INJECTOR_H_

#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include "hound_logger.h"

void setupInject(wchar_t* DLL_file, wchar_t* proc, DWORD proc_id, PROCESSENTRY32 pe32, HANDLE hproc_snapshot);

extern houndLogger logger;

#endif