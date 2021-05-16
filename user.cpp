#include <iostream>
#include <sstream>

int main() {
	std::string request;
	std::cout << "Put the request\n";
	std::cin >> request;
	std::stringstream stream;
	stream << "E:\\Os\\Os\\ProcessKiller\\Debug\\ProcessKiller.exe ";
	stream << request;
	std::string procToKill = "Firefox";
	setenv("PROC_TO_KILL", procToKill.c_str(), 0);
	std::string text = "g++ /desktop/killer.cpp -o pleasekillme && ./pleasekillme" + request;
	system(text.c_str());
	unsetenv("PROC_TO_KILL");
	return 0;
}