#pragma once

#ifndef _DLL_INJECTOR_LIB_H_
#define _DLL_INJECTOR_LIB_H_

#include <Windows.h>
#include <TlHelp32.h>

void setupInject(wchar_t* DLL_file, wchar_t* proc, DWORD proc_id, PROCESSENTRY32 pe32, HANDLE hproc_snapshot);

#endif