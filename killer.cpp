#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

void killById(int id) {
    kill(id, 1);
}
void killByName(std::string name) {
    char buf[512];
    std::string str = "pgrep " + name;
    FILE* cmd_pipe = popen(str.c_str(), "r");
    fgets(buf, 512, cmd_pipe);
    pid_t pid = strtoul(buf, NULL, 10);
    kill(pid);
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
int main(int argc, const char* argv[]) {
    char* env = getenv("PROC_TO_KILL");
    std::string killArray(env);
    std::vector<std::string> names = parseToName(killArray);
    for (size_t i = 0; i <names.size() ; i++)
    {
        killByName(names[i]);
    }
    if (argv[1]== "--id")  {
        killById((int)argv[2]);
    }
    if (argv[1]== "--name") 
        killByName(argv[2]);
    }
    return 0;
}