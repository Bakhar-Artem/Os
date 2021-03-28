g++ -c Number.h -o Number.o
ar rc Number.a Number.o
ranlib Number.a
g++ -g -fPIC -Wall -Werror -Wextra -pedantic Vector.h -shared -o Vector.so
g++ -c ConsoleApplication1.cpp -o ConsoleApplication1.o
g++ -o ConsoleApplication1 ConsoleApplication1.o -L Vector.so
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
./ConsoleApplication1
