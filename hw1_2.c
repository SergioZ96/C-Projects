#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <stdint.h>


/* 

Develop a similar command to the 'head' command reading this form:
	
	./hw1_2 -n #lines pathname

	#lines: the number of lines at the beginning of the file that should be printed out onto the screen.
	pathname: the pathname of the file

*/	
extern int errno;
int main(int argc, char *argv[])
{

	int numlines;
	char pathFile[50];

	//argc will have to be 4, due to 4 arguments needed

	/*
		If there are all the arguments,
	*/
	for(int i = 1; i < 4; ++i)
	{
		if(argv[i] == NULL && argc != 4)
		{
			//Handle error of not enough arguments
			printf("Error: Not Enough Arguments!\n");
			exit(1);
		}
		else
		{

			if(strcmp(argv[i],"-n") == 0)
			{
				sscanf(argv[++i], "%d", &numlines);
				strcpy(pathFile,argv[++i]);
			}
				
		}
	}
	char content[8000];
	char buf[1];
	int fd1 = open(pathFile, O_RDONLY); // fd1 is a file descriptor produced by the open system call

	if (fd1 == -1)
	{
		printf("Error: File %s Does Not Exist! errno = %d\n",pathFile, errno);
		exit(1);
	}
	else
	{
		/* To print out the number of lines from a file, 
		bytes have to continuously be read until it reaches new line character
		'\n', where linecount keeps track of what line it is on and prints content
		*/
		int readBytes, counter = 0;
		int linecount = 0;
		while((readBytes = read(fd1, buf,1)) > 0)
		{
			content[counter] = buf[0];
			++counter;
			if(buf[0]== '\n')
			{
				++linecount;
				if(linecount == numlines)
				{
					for(int j = 0; j < counter; ++j)
					{
						printf("%c",content[j]);
					}
				}
				
			}
		}
		if(linecount < numlines)
		{
			printf("Error: File only has %d lines!\n", linecount);
			exit(1);
		}


	}


	return 0;
}