g++ -c Number.h -o Number.o
ar rcs Number.a Number.o
g++ -g -fPIC -Wall -Werror -Wextra -pedantic Vector.h Number.a -shared -o Vectored.so
gcc -c ConsoleApplication1.cpp -o ConsoleApplication1.o
g++ -o ConsoleApplication1 ConsoleApplication1.o -L Number.a Vector.so
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:. 
./main