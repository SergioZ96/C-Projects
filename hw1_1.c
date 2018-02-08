#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>


/*
	File Copier Program
	Through the command line, takes as input the program, input file for copying,
	and output file, to paste the content onto
	Requirements: 
	Program should only call read() and write() system calls to read and write files.
	  C library functions such as fread() and fwrite() are not allowed

	How much content is read from the file every time (each read() call) should be 
	  determined by an argument of the program. The pathname of the existing file and the 
	  pathname of the new file should also be specified through arguments of the program. 
	  The format of the command for running the program is as follows:

	  ./hw1_1 -n #bytes -i pathname_existing_file -o pathname_new_file  

	  		#bytes: the number of bytes each read() system call reads from the existing file 
	  		(it is also the number of bytes each write() system call writes into the new file).

	  		pathname_existing_file: the pathname of the existing file

			pathname_new_file: the pathname of the new file.

	Your program should be able to handle the following common errors: 1) opening a non_existent file, 
	  2) creating a file in a directory where you have no access.

	The new file should be identical to the existing file when the program finishes running.
*/
extern int errno;
int main(int argc, char *argv[])
{
	// There really are only three arguments we take from the command line: 
	// numbytes (number of bytes), pathExFile(pathname of existing file), pathNewFile(pathname of new file)

	/* For the program to work, argc has to be 7 due to all the arguments */
	int i = 0;
	int numbytes;
	char pathExFile[30];
	char pathNewFile[30];
	
	
	for(int i = 1; i < 7; ++i)
	{
		if(argv[i] == NULL && argc != 7)
		{
			//Handle error of not enough arguments
			printf("Error: Not Enough Arguments!\n");
			exit(1);
		}
		else
		{

			if(strcmp(argv[i],"-n") == 0)
			{
				sscanf(argv[++i], "%d", &numbytes);
			}
			else if(strcmp(argv[i],"-i") == 0)
			{
				strcpy(pathExFile,argv[++i]);
			}
			else if(strcmp(argv[i],"-o") == 0)
			{
				strcpy(pathNewFile,argv[++i]);
			}

				
		}
	}

	char buf[numbytes];
	int fd1 = open(pathExFile, O_RDONLY);
	

	if (fd1 == -1)
	{
		printf("Error: File %s Does Not Exist! errno = %d\n",pathExFile, errno);
		exit(1);
	}
	
	else
	{
		int fd2 = creat(pathNewFile,S_IRWXU);
		if(fd2 == -1)
		{
			printf("Error: Cannot create %s! errno = %d\n",pathNewFile, errno);
			exit(1);
		}

		int n;
		while((n = read(fd1, buf, numbytes)) >= 0)
			write(fd2, buf, n);

		//if( != n)
			//printf("Error: Bytes read does not equal bytes written! errno = %d\n", errno);

		
	}

	
	
	
	return 0;
	// numbytes will be read after program reads -n from command line
	// pathExFile will be read after program reads -i from command line
	// pathNewFile is the pathname for the new file that will be constructed. Will be read after -o is read


    /* Error Handling:
	   - If pathExFile is non-existent, then handle with "File Does Not Exist"
	   - If you cannot create a file in a directory with the pathNewFile, then handle with "Cannot create file in ...(pathNewFile)..."
	*/
}
