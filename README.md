myShell
=======
d2chau
10/11/2013

	The command line interpreter, called a shell, is an application program
	that gets commands from a keyboard and uses the system call interfaces
	to invoke OS functions.

IMPLEMENTATION DETAILS
	
	Programming language:  C++
	
	This program was developed using Vmware ubuntu 12.04 and
	csegrid.ucdenver.pvt.The main computing environment is a Windows 7 
	64-bit machine.

SHELL SUPPORT

	(1) exit or stop shell commands
	(2) Commands with no arguments
	(3) Commands with arguments
	(4) Commands with without arguments, whose output is redirected to a file
	(4) Commands, with or without arguments, whose input is redirected from a
		file
	(f) Display error messages if the typed command has errors or canâ€™t be
		found

FILES

	(1) main.cpp
	(2) shell.cpp
	(3) shell.h
	(4) enums.h
	(5) makefile
	(6) readme.txt

TO RUN

	(1) Untar the hau1551.tar file by typing the following at the 
	command prompt:

		tar xvf hau1551.tar

	(2) Run the make command

		make makefile

	(3) Execute the program

		./myShell

	(4) Exit the shell

		exit
