#include <stdio.h>
/*
	Write a C program with C library functions, which prints out the first part of a text file onto screen, similar to the head tool in Linux. 
	Basically, you are asked to rewrite the program in homework 1-2. The new program uses C library functions (fread(), fgetc(), etc). 

	Detailed requirements:
	1. Name the program hw2_1.c.  The grader may use a script to find and test your programs. The script will not find your program if the program has a different name.

	2. The program should NOT call read() system call or pread() system call to read the file. C library functions for reading files, such as fread() and fgetc(), must be used.

	4. The program should analyze arguments in the command line, in order to determine 1) which file to open and 
	   2) how many lines at the beginning of the file should be printed out onto screen. The format of the command for running the program is as follows 
	   (Important! If you don't follow strictly, the script testing your program may not be able to run your program correctly, and you may get lower grades).

	./hw2_1   -n    #lines   pathname

	#lines: the number of lines at the beginning of the file that should be printed out onto the screen.

	pathname: the pathname of the file
*/

int main (int argc, char *argv[])
{
	int numLines;
	
}