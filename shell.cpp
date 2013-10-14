/*
*   Author: David Chau
*   File: shell.cpp
*   Description: Shell implementation file for unix/linux OS's
*/

#include "shell.h"
shell::shell()
{
	mode = NORMAL;
	userInput = (char*)malloc(sizeof(char)*MAX_INPUT);
	len = MAX_INPUT;
}

void shell::runShell()
{
	char *supplement;
	char *cmdArgv[MAX_INPUT];

	while(true)
	{
		mode = NORMAL;
		getcwd(CWD, MAX_PATH);
		std::cout<< CWD << ">$ ";
		getline(&userInput, &len, stdin);

		//Seeing if the user entered exit. If they did, then exit normally
		if(strcmp(userInput, "exit\n") == 0)
			exit(1);

		parse(userInput, cmdArgv, &supplement);

		if(strcmp(*cmdArgv, "cd") == 0)
		{
			if(cmdArgv[1] == NULL)
				chdir(getenv("HOME"));
			else
				chdir(cmdArgv[1]);
		}
		else 
			execute(cmdArgv, &supplement);
	}
}

int shell::parse(char *inputString, char *cmdArgv[], char **supplementPtr)
{
	int cmdArgc = 0;
	bool stopParsing = false;
	char *inputCharPtr = inputString;
	//Going through the entire input line
	while(*inputCharPtr != '\0' && stopParsing == false)
	{
		*cmdArgv = inputCharPtr;
		cmdArgc++;

		//Iterating though all alpha-numeric characters
		while((isspace(*inputCharPtr) == 0) && *inputCharPtr != '\0' 
				&& stopParsing == false)
		{
			switch(*inputCharPtr)
			{
				case '&':
					mode = BACKGROUND;
					break;
				case '>':
				case '<':
					if(*inputCharPtr == '>')
						mode = OUTPUT_REDIRECTION;
					else
						mode = INPUT_REDIRECTION;

					*cmdArgv = '\0';
					inputCharPtr++;

					//Iterating the input until the next argument
					while(*inputCharPtr == ' ' || *inputCharPtr == '\t')
						inputCharPtr++;
					*supplementPtr = inputCharPtr;
					nullDelimateArgs(*supplementPtr);
					stopParsing = true;
					break;
				case '|':
					mode = PIPELINE;
					*cmdArgv = '\0';
					inputCharPtr++;

					//Iterating the input until the next argument
					while(*inputCharPtr == ' ' || *inputCharPtr == '\t')
						inputCharPtr++;
					*supplementPtr = inputCharPtr;
					stopParsing = true;
					break;
				default:
					//This should have nothing in it because we don't need
					//to do anything while scanning the input.
					break;
			}
			inputCharPtr++;
		}
		//Going to the next space and replacing all non-alpha characters
		//with a null character
		while((isspace(*inputCharPtr) != 0) && stopParsing == false)
		{
			*inputCharPtr = '\0';
			inputCharPtr++;
		}
		cmdArgv++;
	}
	*cmdArgv = '\0';
	return cmdArgc;
}

void shell::nullDelimateArgs(char *inputCharPtr)
{
	//Going to the end of the word
	while(*inputCharPtr != ' ' && *inputCharPtr != '\t' && *inputCharPtr != '\n')
		inputCharPtr++;

	//Making the last argument be a null character
	*inputCharPtr = '\0';
}

void shell::execute(char **cmdArgv, char **addArgsPtr)
{
    //Making two process ids
    pid_t pid, pid2;
    FILE *fp;
    int status;

    //Creating pipes so that threads can share info through IPC's
    int fd[2];
    char *cmdArgv2[MAX_INPUT];
    char *addArgs2 = NULL;
    
    if(mode == PIPELINE)
    {
        if(pipe(fd))                    //create pipe
        {
            std::cerr<< "An error occured making a Pipe failed!";
            exit(-1);
        }
        parse(*addArgsPtr, cmdArgv2, &addArgs2);
    }

    pid = fork();
    if( pid < 0)
    {
        std::cerr<< "An error occured creating a child process.\n";
        exit(-1);
    }
    //Child process
    else if(pid == 0)
    {
        switch(mode)
        {
            case OUTPUT_REDIRECTION:
                //Opens the file and gives write/update privileges
                fp = fopen(*addArgsPtr, "w+");
                dup2(fileno(fp), 1);
                break;
            case INPUT_REDIRECTION:
                //Opens the file and gives read access
                fp = fopen(*addArgsPtr, "r");
                dup2(fileno(fp), 0);
                break;
            case PIPELINE:
                //close input of pipe
                close(fd[0]);
                //Sending data to another process
                dup2(fd[1], fileno(stdout));
                close(fd[1]);
                break;
        }
        if(execvp(*cmdArgv, cmdArgv) == -1)
            std::cout<<"The command is not supported\n";
    }
    //Parent process
    else
    {
        if(mode == BACKGROUND)
            ;
        else if(mode == PIPELINE)
        {
            //wait for child process 1 to finish
            waitpid(pid, &status, 0);
            pid2 = fork();
            if(pid2 < 0)
            {
                std::cerr<< "There was an error forking the second process.\n";
                exit(-1);
            }
            else if(pid2 == 0)
            {
                //close output to pipe
                close(fd[1]);
                //Reading input from other process
                dup2(fd[0], fileno(stdin));
                close(fd[0]);
                if(execvp(*cmdArgv2, cmdArgv2) == -1)
                    std::cout<<"The command is not supported\n";
            }
            else
            {
                close(fd[0]);
                close(fd[1]);
            }
        }
        else
            waitpid(pid, &status, 0);
    }
}