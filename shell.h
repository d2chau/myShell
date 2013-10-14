/*
*   Author: David Chau
*   File: shell.h
*   Description: Header file to support implementation
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#include "enums.h"

class shell
{
public:
    //Default constructor
    shell();

    //Runs the shell program
    void runShell();

protected:

    //Parses input into the first argument and additional arguments
    //Sees if there are any pipes, indirection, or redirection
    //Returns number of arguments if needed
    int parse(char *inputString, char *cmdArgv[], char **supplementPtr);
    
    //Goes to the end of the word and delimits it with a null character
    void nullDelimateArgs(char *srcPtr);

    //Function which executes OS calls
    void execute(char **cmdArgv, char **supplementPtr);

private:
    modes mode;
    char CWD[MAX_PATH];
    char *userInput;
    size_t len;
};