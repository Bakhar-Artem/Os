#include <iostream>
#include <string>
#include <unistd.h>
const std::string proc[] = { "./m","./a","./p","./s"};
const int stopWord = -1;
int main() {
int p[2];
    for (int i = 0; i < 3; i++) {
	dup2(p[1],1);
        pipe(p);
        if (fork()==0) {
            execlp(proc[i].c_str(),proc[i].c_str(), NULL);
	    
        }
	dup2(p[0],0);
	close(p[1]);
    }

execlp(proc[3].c_str(),proc[3].c_str(), NULL);
exit(0);
return 0;
}
