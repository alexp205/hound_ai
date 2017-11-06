#include "main.h"
using namespace std;

//LPCWSTR file_location = TEXT("C:\\Users\\ap\\Documents\\Games\\Emulators\\dolphin-master-5.0-321-x64\\Dolphin-x64\\Dolphin.exe");
wstring create_proc_command = L"C:\\Users\\ap\\Documents\\Games\\Emulators\\dolphin-master-5.0-321-x64\\Dolphin-x64\\Dolphin.exe -e \"C:\\Users\\ap\\Documents\\Games\\Emulators\\ROMs\\Super Smash Bros. Melee (v1.02).iso\"";
wstring log_path = L"C:\\Users\\ap\\Documetns\\Projects\\Programs\\logs\\hound_log.txt";
LPSTARTUPINFOW si;
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

bool mainSetup()
{
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si->cb = sizeof(si);
	vector<wchar_t> string_converter(create_proc_command.begin(), create_proc_command.end());
	string_converter.push_back(0);

	//start Dolphin
	bool target_opened = CreateProcessW(NULL,
		string_converter.data(),
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		si,
		&pi);
	if (!target_opened) return false;

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	logger.initLogger();

	return true;
}

int main()
{
	try
	{
		wcout << L"Welcome to Hound_AI" << endl;
		wcout << L"Now setting up program and target\n" << endl;

		//setup
		bool setup_complete = mainSetup();
		if (!setup_complete) {
			DWORD err = GetLastError();
			log_msg = L"Failed Dolphin setup with error code: " + err;
			logger.logError(log_msg);
			wstring err_msg = parseWinError(err);
			log_msg = L" " + err_msg;
			logger.logError(log_msg);
			return onExit(-1);
		}
		DWORD pid = pi.dwProcessId;

		log_msg = L"Dolphin PID: " + pid;
		logger.logInfo(log_msg);

		//inject DLL
		log_msg = L"Now injecting Hound_DLL into Dolphin";
		logger.logInfo(log_msg);

		PROCESSENTRY32W pe32 = { sizeof(PROCESSENTRY32W) };
		HANDLE hproc_snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		setupInject(L"hound_DLL.dll", L"Dolphin.exe", pid, pe32, hproc_snap);

		//access mapped file
		LPCSTR mapfile_name = "hound_DLL_file_map";
		HANDLE hmapfile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, mapfile_name);
		if (hmapfile == NULL) {
			log_msg = L"Failed opening mapped file: " + parseWinError(GetLastError());
			logger.logError(log_msg);
			return onExit(-1);
		}
		LPCWSTR mapbuf = (LPCWSTR)MapViewOfFile(hmapfile, FILE_MAP_ALL_ACCESS, 0, 0, 256);
		if (mapbuf == NULL) {
			log_msg = L"Failed accessing view of mapped file: " + parseWinError(GetLastError());
			logger.logError(log_msg);
			return onExit(-1);
		}

		//NOTE: some other form of IPC may be necessary if mapped memory communication is too difficult/inefficient/etc.
		//setup Dolphin I/O
		//TODO: setup Dolphin I/O

		//TODO: continue...
	}
	catch (exception& e)
	{
		string exception_string = e.what();
		int size_needed = MultiByteToWideChar(CP_UTF8, 0, &exception_string[0], (int)exception_string.size(), NULL, 0);
		wstring exception_msg(size_needed, 0);
		MultiByteToWideChar(CP_UTF8, 0, &exception_string[0], (int)exception_string.size(), &exception_msg[0], size_needed);
		log_msg = L"Standard exception: " + exception_msg;
		logger.logAlarm(log_msg);

		return onExit(-1);
	}

	return onExit(0);
}