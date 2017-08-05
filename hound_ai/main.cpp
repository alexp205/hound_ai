#include <Windows.h>
#include <cstdio>

//LPCWSTR file_location = TEXT("C:\\Users\\ap\\Documents\\Games\\Emulators\\dolphin-master-5.0-321-x64\\Dolphin-x64\\Dolphin.exe");
wchar_t create_proc_command[256] = TEXT("C:\\Users\\ap\\Documents\\Games\\Emulators\\dolphin-master-5.0-321-x64\\Dolphin-x64\\Dolphin.exe -e \"C:\\Users\\ap\\Documents\\Games\\Emulators\\ROMs\\Super Smash Bros. Melee (v1.02).iso\"");
STARTUPINFO si;
PROCESS_INFORMATION pi;

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
	printf("Welcome to Hound_AI\n");
	printf("Now setting up program and target\n\n");
	
	//setup
	bool setup_complete = mainSetup();
	if (!setup_complete) {
		printf("Failed Dolphin setup with error code: %d\n", GetLastError());
		return -1;
	}
	DWORD pid = pi.dwProcessId;

	printf("Dolphin PID: %ld\n", pid);

	//inject DLL and access mapped file


	//TODO: continue

	return 0;
}