#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <windows.h>
#include <string>
#include <Tlhelp32.h>
#include <sstream>

bool killById(int id) {
	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0, id);
	bool result = false;
	if (hProcess != NULL) {
		result =TerminateProcess(hProcess, 1);
		CloseHandle(hProcess);
	}
	return result;
}
void killByName(std::string name) {
    HANDLE h_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
    PROCESSENTRY32 proc;
    proc.dwSize = sizeof(proc);
    BOOL h_res = Process32First(h_snapshot, &proc);
    while (h_res) {
        wchar_t* wc = new wchar_t[name.size() + 1];
        mbstowcs(wc, name.c_str(), name.size() + 1);
        if (wcscmp(proc.szExeFile, wc) == 0) {
            HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0, (DWORD)proc.th32ProcessID);
            if (hProcess != NULL) {
                TerminateProcess(hProcess,1);
                CloseHandle(hProcess);
            }
        }
        h_res = Process32Next(h_snapshot, &proc);
    }
    CloseHandle(h_snapshot);
}
std::vector<std::string>& parseToName(std::string names) {
    std::vector<std::string> parsed;
    int index = 0;
    for (size_t i = 0; i < names.size(); i++)
    {
        if (names[i] == ',') {
            names[i] = ' ';
        }
    }
    std::stringstream stream(names);
    while (!stream.eof()) {
        std::string prog;
        stream >> prog;
        parsed.push_back(prog);
    }
    return parsed;
}
int main(int argc,const char* argv[]) {
    const CHAR* name = "PROC_TO_KILL";
    const DWORD buffSize = 100;
    char buffer[buffSize];
    GetEnvironmentVariableA(name, buffer, buffSize);
    std::string env(buffer);
    std::vector<std::string> namesToKill = parseToName(env);
    for (size_t i = 0; i < namesToKill.size(); i++)
    {
        killByName(namesToKill[i]);
    }
    if (argv[0] == "--id") {
        killById((int)argv[1]);
    }
    else {
        killByName(argv[1]);
    }
	return 0;
}