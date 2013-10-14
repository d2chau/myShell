myShell: main.o shell.o
	g++ main.o shell.o -o myShell
main.o: main.cpp shell.h
	g++ -c main.cpp
shell.o: shell.cpp shell.h enums.h
	g++ -c shell.cpp
clean:
	rm -rf *o shellLab
