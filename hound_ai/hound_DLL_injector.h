#pragma once

#ifndef _HOUND_DLL_INJECTOR_H_
#define _HOUND_DLL_INJECTOR_H_

#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>

void setupInject(wchar_t* DLL_file, wchar_t* proc, DWORD proc_id, PROCESSENTRY32 pe32, HANDLE hproc_snapshot);

#endif