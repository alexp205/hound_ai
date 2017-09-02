#include "main.h"
#include <iostream>

//LPCWSTR file_location = TEXT("C:\\Users\\ap\\Documents\\Games\\Emulators\\dolphin-master-5.0-321-x64\\Dolphin-x64\\Dolphin.exe");
wchar_t create_proc_command[256] = TEXT("C:\\Users\\ap\\Documents\\Games\\Emulators\\dolphin-master-5.0-321-x64\\Dolphin-x64\\Dolphin.exe -e \"C:\\Users\\ap\\Documents\\Games\\Emulators\\ROMs\\Super Smash Bros. Melee (v1.02).iso\"");
STARTUPINFO si;
PROCESS_INFORMATION pi;

//Usage:
// - 0 = success (no errors), follow standard exit procedures
// - any other integer = error, execute error procedures before exit (if possible)
bool onExit(int exit_status)
{
	int exit_code;
	if (exit_status == 0) {
		exit_code = 0;
	}
	else {
		exit_code = -1;
	}

	switch (exit_code) {
	case 0: 
		//TODO: success procedure here
		break;
	case -1: 
		//TODO: error procedure here
		break;
	}

	return exit_code;
}

bool initLogger()
{
	bool init_success;

	//TODO: setup logger here (source: http://www.drdobbs.com/cpp/logging-in-c/201804215?pgno=1)

	return init_success;
}

bool mainSetup()
{
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(si);

	//start Dolphin
	bool target_opened = CreateProcessW(NULL,
		create_proc_command,
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&si,
		&pi);
	if (!target_opened) return false;

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return true;
}

int main()
{
	std::wcout << L"Welcome to Hound_AI" << std::endl;
	std::wcout << L"Now setting up program and target\n" << std::endl;
	
	//setup
	bool setup_complete = mainSetup();
	if (!setup_complete) {
		DWORD err = GetLastError();
		std::wcout << L"Failed Dolphin setup with error code: " << err;
		std::wstring err_msg = parseWinError(err);
		std::wcout << " " << err_msg << std::endl;
		return onExit(-1);
	}
	DWORD pid = pi.dwProcessId;

	std::wcout << L"Dolphin PID: " << pid << std::endl;

	//inject DLL
	std::wcout << L"Now injecting Hound_DLL into Dolphin" << std::endl;

	PROCESSENTRY32 pe32 = { sizeof(PROCESSENTRY32) };
	HANDLE hproc_snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	setupInject(L"hound_DLL.dll", L"Dolphin.exe", pid, pe32, hproc_snap);

	//access mapped file
	std::wstring mapfile_name = L"hound_DLL_file_map";
	HANDLE hmapfile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, mapfile_name.c_str());
	if (hmapfile == NULL) {
		std::wcout << L"Failed opening mapped file: " << parseWinError(GetLastError()) << std::endl;
		return onExit(-1);
	}
	LPCWSTR mapbuf = (LPCWSTR) MapViewOfFile(hmapfile, FILE_MAP_ALL_ACCESS, 0, 0, 256);
	if (mapbuf == NULL) {
		std::wcout << L"Failed accessing view of mapped file: " << parseWinError(GetLastError()) << std::endl;
		return onExit(-1);
	}

	//setup named pipes


	//TODO: continue

	return onExit(0);
}