#include <Windows.h>
#include <iostream>
#include <sstream>
int main() {
	std::string request;
	std::cout << "Put the request\n";
	std::cin >> request;
	const CHAR* name = "PROC_TO_KILL";
	const CHAR* value = "Discord.exe,explorer.exe";
	const DWORD buffSize = 100;
	char buffer[buffSize];
	SetEnvironmentVariableA(name, value);
	GetEnvironmentVariableA(name, buffer, buffSize);
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi = { 0 };
	std::stringstream stream;
	stream << "E:\\Os\\Os\\ProcessKiller\\Debug\\ProcessKiller.exe ";
	stream << request;
	CreateProcess(NULL, (LPWSTR) stream.str().c_str() , NULL, NULL, FALSE, NULL, NULL, NULL, &si, &pi);
	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);
	SetEnvironmentVariableA(name, NULL);
	return 0;
}