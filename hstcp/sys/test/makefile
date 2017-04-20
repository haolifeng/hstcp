all:clean main
main:main.o thread.o time_tools.o
	g++ -o main main.o thread.o time_tools.o -lpthread

main.o:main.cpp
	g++ -c -g main.cpp
thread.o:../thread.cpp
	g++ -c -g ../thread.cpp
time_tools.o:../time_tools.o
	g++ -c -g ../time_tools.cpp

clean:
	rm -rf *.o main
