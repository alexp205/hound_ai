#include "hound_DLL_injector.h"
using namespace std;

typedef HINSTANCE(*fpLoadLibrary)(char*);

void setupInject(wchar_t* DLL_file, wchar_t* proc, DWORD proc_id, PROCESSENTRY32W pe32, HANDLE hproc_snapshot)
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
}

bool InjectDLL(wchar_t* DLL_file, DWORD proc_id)
{
	HANDLE hproc;
	LPVOID param_addr;

	//DLL injection pre-setup
	HINSTANCE hDLL = LoadLibraryW(L"KERNEL32");

	fpLoadLibrary load_library_addr = (fpLoadLibrary)GetProcAddress(hDLL, "LoadLibraryW");

	//setup process for injection
	hproc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, proc_id);

	wchar_t dll_path[256] = L"C:\\Users\\ap\\Documents\\Projects\\Programs\\DLLs\\";

	wcscat_s(dll_path, DLL_file);

	param_addr = VirtualAllocEx(hproc, 0, (wcslen(dll_path) + 1) * sizeof(wchar_t), MEM_COMMIT, PAGE_READWRITE);
	bool mem_written = WriteProcessMemory(hproc, param_addr, dll_path, (wcslen(dll_path) + 1) * sizeof(wchar_t), NULL);

	//inject DLL
	CreateRemoteThread(hproc, nullptr, 0, LPTHREAD_START_ROUTINE(load_library_addr), param_addr, 0, 0);

	CloseHandle(hproc);
	
	return mem_written;
}