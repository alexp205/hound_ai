#include "hound_DLL_injector.h"
using namespace std;

void setupInject(char* DLL_file, wchar_t* proc, DWORD proc_id, PROCESSENTRY32W pe32);
bool InjectDLL(char* DLL_file, DWORD proc_id);

HANDLE hproc_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

void setupInject(char* DLL_file, wchar_t* proc, DWORD proc_id, PROCESSENTRY32W pe32)
{
	while (!proc_id) {
		wstring proc_name_str(proc);
		log_msg = L"Searching for " + proc_name_str + L"...";
		logger.logInfo(log_msg);
		log_msg = L"Make sure process is running";
		logger.logInfo(log_msg);

		if (Process32FirstW(hproc_snapshot, &pe32)) {
			do {
				if (!wcscmp(pe32.szExeFile, proc)) {
					proc_id = pe32.th32ProcessID;
					break;
				}
			} while (Process32NextW(hproc_snapshot, &pe32));
		}
		Sleep(1000);
	}

	while (!InjectDLL(DLL_file, proc_id))
	{
		log_msg = L"DLL failed to inject";
		logger.logError(log_msg);
		Sleep(1000);
	}

	log_msg = L"DLL injected successfully\n";
	logger.logInfo(log_msg);
	log_msg = L"Closing injector in 5 seconds";
	logger.logInfo(log_msg);

	CloseHandle(hproc_snapshot);
}

bool InjectDLL(char* DLL_file, DWORD proc_id)
{
	HANDLE hproc;
	LPVOID param_addr;

	//DLL injection pre-setup
	HINSTANCE hDLL = LoadLibrary(L"KERNEL32");
	if (hDLL == 0) {
		log_msg = L"LoadLibrary failed with " + parseWinError(GetLastError());
		logger.logError(log_msg);
		return false;
	}

	HMODULE load_library_addr = (HMODULE)GetProcAddress(hDLL, "LoadLibraryA");
	if (load_library_addr == 0) {
		log_msg = L"GetProcAddress failed with " + parseWinError(GetLastError());
		logger.logError(log_msg);
		FreeLibrary(load_library_addr);
		return false;
	}

	//setup process for injection
	hproc = OpenProcess(PROCESS_ALL_ACCESS, 0, proc_id);
	if (hproc == 0) {
		log_msg = L"OpenProcess failed with " + parseWinError(GetLastError());
		logger.logError(log_msg);
		FreeLibrary(load_library_addr);
		return false;
	}

	string dll_path = "C:\\Users\\ap\\Documents\\Projects\\DLLs\\";

	dll_path += DLL_file;

	param_addr = VirtualAllocEx(hproc, 0, dll_path.size() + 1, MEM_COMMIT, PAGE_READWRITE);
	if (param_addr == 0) {
		log_msg = L"VirtualAllocEx failed with " + parseWinError(GetLastError());
		logger.logError(log_msg);
		FreeLibrary(load_library_addr);
		CloseHandle(hproc);
		return false;
	}
	bool mem_written = WriteProcessMemory(hproc, param_addr, dll_path.c_str(), dll_path.size() + 1, 0);
	if (!mem_written) {
		log_msg = L"WriteProcessMemory failed with " + parseWinError(GetLastError());
		logger.logError(log_msg);
		FreeLibrary(load_library_addr);
		VirtualFreeEx(hproc, param_addr, 0, MEM_RELEASE);
		CloseHandle(hproc);
		return false;
	}

	//inject DLL
	HANDLE hthread = CreateRemoteThread(hproc, nullptr, 0, (LPTHREAD_START_ROUTINE)load_library_addr, param_addr, 0, 0);
	if (hthread == 0) {
		log_msg = L"CreateRemoteThread failed with " + parseWinError(GetLastError());
		logger.logError(log_msg);
		FreeLibrary(load_library_addr);
		VirtualFreeEx(hproc, param_addr, 0, MEM_RELEASE);
		CloseHandle(hproc);
		return false;
	}

	WaitForSingleObject(hthread, INFINITE);
	DWORD exit_code = 0;
	if (GetExitCodeThread(hthread, &exit_code)) {
		log_msg = L"General exit code with " + exit_code;
		logger.logInfo(log_msg);
	}
	else {
		log_msg = L"GetExitCodeThread failed with " + parseWinError(GetLastError());
		logger.logError(log_msg);
	}

	CloseHandle(hthread);
	FreeLibrary(hDLL);
	VirtualFreeEx(hproc, param_addr, 0, MEM_RELEASE);
	CloseHandle(hproc);

	return true;
}