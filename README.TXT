XSHELL
CSCE 361
Group 1
Hall, Laney, Kenney, Haynatzki

The xshell command shell is used in place of a similar command shell like BASH. It can either be set as a login shell for a user of a unix system, or it can be run within another command shell.

Setup directions:
1) From the main directory, run the "make" command to build the system. 
2) Run "./xsh" to start the system.

Running instructions:
The system currently accepts several standard commands listed below.

ls - lists the files in the working directory
cd arg - changes the working directory to the directory specified by arg
clear - clears the screen
${{math}} - calculates the value of the expression specified by math
pwd - outputs the path of the current directory
echo arg - outputs the result of the argument
man - manual entry for a job
exit - exits the system

Several other external commands are functional, but not listed here for brevity.

Code structure:
The lexer and parser code can be found in xsh_lex.l and xsh_parse.y, respectively. xsh_main.c contains all primary interpreting functionality and points outward to functions in the other files. Files are named according to functionality.
