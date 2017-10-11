#include "hound_DLL_injector.h"

using namespace std;
void setupInject(wchar_t* DLL_file, wchar_t* proc, DWORD proc_id, PROCESSENTRY32 pe32, HANDLE hproc_snapshot);
bool InjectDLL(wchar_t* DLL_file, DWORD proc_id);

wchar_t file_to_inject[256];
wchar_t proc_name[256];

typedef HINSTANCE(*fpLoadLibrary)(char*);

void setupInject(wchar_t* DLL_file, wchar_t* proc, DWORD proc_id, PROCESSENTRY32 pe32, HANDLE hproc_snapshot)
{
	while (!proc_id) {
		wstring proc_name_str(proc_name);
		logger.logInfo(L"Searching for " + proc_name_str + L"...");
		logger.logInfo(L"Make sure process is running");

		if (Process32First(hproc_snapshot, &pe32)) {
			do {
				if (!wcscmp(pe32.szExeFile, proc_name)) {
					proc_id = pe32.th32ProcessID;
					break;
				}
			} while (Process32Next(hproc_snapshot, &pe32));
		}
		Sleep(1000);
	}

	while (!InjectDLL(DLL_file, proc_id))
	{
		logger.logError(L"DLL failed to inject");
		Sleep(1000);
	}

	logger.logInfo(L"DLL injected successfully\n");
	logger.logInfo(L"Closing injector in 5 seconds");
}

bool InjectDLL(wchar_t* DLL_file, DWORD proc_id)
{
	HANDLE hproc;
	LPVOID param_addr;

	//DLL injection pre-setup
	HINSTANCE hDLL = LoadLibrary(L"KERNEL32");

	fpLoadLibrary load_library_addr = (fpLoadLibrary)GetProcAddress(hDLL, "LoadLibraryA");

	//setup process for injection
	hproc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, proc_id);

	wchar_t dll_path[256] = L"C:\\Users\\ap\\Documents\\Projects\\Programs\\DLLs\\";

	wcscat_s(dll_path, DLL_file);

	param_addr = VirtualAllocEx(hproc, 0, wcslen(dll_path) + 1, MEM_COMMIT, PAGE_READWRITE);
	bool mem_written = WriteProcessMemory(hproc, param_addr, dll_path, wcslen(dll_path) + 1, NULL);

	//inject DLL
	CreateRemoteThread(hproc, 0, 0, (LPTHREAD_START_ROUTINE)load_library_addr, param_addr, 0, 0);

	CloseHandle(hproc);
	
	return mem_written;
}