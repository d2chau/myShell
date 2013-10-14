/*
*   Author: David Chau
*   File: enums.h
*   Description: File that contains structures and definitions
*/

//Max history path of user's location
#define MAX_PATH    100
//Max number of characters that can be entered by the user
#define MAX_INPUT   80

//Different Modes which can occur
//Dependent on user input command
enum modes
{
    NORMAL, OUTPUT_REDIRECTION, INPUT_REDIRECTION, 
    PIPELINE, BACKGROUND
};