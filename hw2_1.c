#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
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

/*
bool file_exists(const char * filename)
{
	if (FILE * file = fopen(filename, "r"))
	{
		fclose(file);
		return true;
	}
	return false;
}
*/
extern int errno;
int main (int argc, char *argv[])
{
	FILE *fp; //file pointer 
	int numLines;
	char pathFile[100]; //arrays with initialized size
	char buf[1000];

	// Loop through the arguments
	for(int i = 1; i < 4; ++i)
	{
		//Error if there aren't four arguments
		if(argc != 4)
		{
			printf("Error: Not enough arguments!\n");
			exit(1);
		}
		else
		{
			
			// Compare the second argument to see if it equal to '-n'
			if(strcmp(argv[i],"-n") == 0)
			{
				//Store the third element in argv as an int, into numLines
				sscanf(argv[++i], "%d", &numLines);
				//Copy fourth element from argv which is the pathname to 'pathFile'
				strcpy(pathFile,argv[++i]);
			}
			else 
			{
				printf("Incorrect command: %s not recognized\n",argv[i]);
				exit(1);
			}
				
		}
	}

	fp = fopen(pathFile, "r");
	if(fp == NULL)
	{
		printf("Error: Cannot open file %s\n", pathFile);
		exit(1);
	}
	char c;
	while(numLines > 0)
	{
		c = fgetc(fp);
		if (feof(fp))
			break;
		if(c == '\n')
			--numLines;

		printf("%c", c);
	}

	fclose(fp);
	return(0);
/*
	if (file_exists(pathFile) == true){
		FILE *fp = fopen(filename,"r");
		for ()
	}
*/

}