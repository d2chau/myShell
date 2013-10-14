/*
*   Author: David Chau
*   File: main.cpp
*   Description: Driver main file.
*   This program is a shell program made for unix/linux systems
*/

#include "shell.h"

int main(int argc, char *argv[])
{
    shell myShell;
    myShell.runShell();
    return 0;
}