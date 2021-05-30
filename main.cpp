#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
const std::string proc[] = { "./m","./a","./p","./s"};
const int stopWord = -1;
int main() {
int p[2];
    for (int i = 0; i < 3; i++) {
        pipe(p);
	int pid=fork();
        if (pid==0) {
	dup2(p[1],1);
        execlp(proc[i].c_str(),proc[i].c_str(), NULL);
	exit(0);
        }
	dup2(p[0],0);
	close(p[1]);
	waitpid(pid,nullptr,0);
    }
execlp(proc[3].c_str(),proc[3].c_str(), NULL);
exit(0);

return 0;
}
