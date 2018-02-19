#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/mman.h>

#define NUMCHARS (1000)
#define FILESIZE (NUMCHARS * sizeof(char))

/*
	Write a C program using mmap(), which prints out the first part of a text file onto screen, similar to the head tool in Linux. 
	Basically, you are asked to rewrite the program in homework 1-2 or homework 2-1. The new program uses mmap() to map file to 
	virtual memory and then accesses the data in the file with a pointer.

Detailed requirements:

	1. Name the program hw2_2.c.  The grader may use a script to find and test your programs. 
	The script will not find your program if the program has a different name.

	2. The program should NOT call any functions (e.g., fread(), fgetc(), etc ) or system calls (read() and pread() ) to read the file. 
	Instead, the program must use mmap() to map the file into virtual memory and then use the pointer obtained from mmap() to read the file. 
	Programs that fail to follow this rule will not be graded.

	4. The program should analyze arguments in the command line, in order to determine 1) which file to open and 2) how many lines at the beginning of the file 
	should be printed out onto screen. The format of the command for running the program is as follows (Important! If you don't follow strictly, 
	the script testing your program may not be able to run your program correctly, and you may get lower grades).

	./hw2_2   -n    #lines   pathname

	#lines: the number of lines at the beginning of the file that should be printed out onto the screen.

	pathname: the pathname of the file

	Hint: you may use getopt() function in this part.

	5. The output should be identical to the output of head tool with the same arguments.
*/

int main (int argc, char *argv[])
{
	//map = mmap(0, FILESIZE, PROT_READ, MAP_SHARED, fd, 0);
	int fd, numLines, k;
	char pathFile[100];
	char *map;

	//loop for going through the command line arguments 
	int i;
	for(i = 1; i < 4; ++i)
	{
		if(argc != 4)
		{
			printf("Error: There are not enough arguments!\n");
			exit(1);
		}
		else
		{
			if(strcmp(argv[i],"-n") == 0)
			{
				sscanf(argv[++i],"%d", &numLines);
				strcpy(pathFile, argv[++i]);
			}
			else
			{
				printf("Incorrect command: %s not recognized",argv[i]);
				exit(1);
			}
		}
	}

	fd = open(pathFile,O_RDONLY);
	if(fd < 0)
	{
		printf("Could not open %s\n", pathFile);
		exit(1);
	}

	map = mmap(0,FILESIZE,PROT_READ,MAP_SHARED,fd,0);
	int lineCounter = 0;

	if(map == MAP_FAILED)
	{
		close(fd);
		printf("Error mmapping the file");
		exit(1);
	}
	
	for(k = 0; k <=NUMCHARS; ++k)
	{

		if(map[k] == '\n' || map[k] == '\0')
		{
			++lineCounter;
			if(lineCounter == numLines)
			{
				int j;
				for(j = 0; j < k; ++j)
				{
					printf("%c",map[j]);
				}
				printf("\n");
			}
		}

	}
	if(lineCounter < numLines)
	{
		printf("Error: File only has %d lines in it.\n", lineCounter);
		exit(1);
	}

	

	if(munmap(map, FILESIZE) == -1)
	{
		printf("Error un-mapping the file");
		exit(1);
	}

	close(fd);
	return 0;
}