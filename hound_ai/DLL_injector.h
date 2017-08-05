#pragma once

#ifndef _DLL_INJECTOR_H_
#define _DLL_INJECTOR_H_

#include <Windows.h>
#include <TlHelp32.h>

void setupInject(wchar_t* DLL_file, wchar_t* proc, DWORD proc_id, PROCESSENTRY32 pe32, HANDLE hproc_snapshot);
bool InjectDLL(DWORD proc_id);

#endif