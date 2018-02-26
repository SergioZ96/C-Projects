/*	CS 288-102
	Program written by:

		Sergio Zurita
		Computer Science Undergraduate at NJIT 2018



	Instructions:

	Write a C program to calculate the total size of the files in a directory 
	and the sub-directories of the directory.
	
	Note that the total size should include the sizes of sub-directories 
	(directories are also files), but not the size of the top-level directory.
	
	If a file is a symbolic link, the symbolic link should not be dereferenced. 
	So the size of the symbolic link is added to the total size, not the size of 
	the file pointed by the link.

	The pathname of the directory is specified in the command line. 
	The format of the command for running the program is as follows 

		./your_program    pathname_of_directory

	To facilitate testing, your program should print out ONLY the total size in bytes,
	i.e., ONE number, nothing else.

	The result should match with the result of the 'du' command of the directory
*/



#include <stdio.h>
#include <stdlib.h>
#include <sys/dirent.h>
#include <sys/stat.h>
#include <sys/dirent.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>


extern int errno;

int main (int argc, char* argv[])
{
	/* First order of business:
		1) understand that our command line arguments (excluding the executable)
		has one argument, the path name of directory

		2) We have to look through the directory for its files and subdirectories
		, add their sizes and output a total size of the directory in bytes

	*/

	// Look at getdents(...), int stat(...), int fstat(...), etc.
	int fd;
	struct stat st;

	if(argc == 2) /* Handles making file descriptor if there are enough arguments */
	{
		fd = open(argv[1], O_RDONLY);
		if(fd == -1)
		{
			printf("Error: Cannot find %s. errno = %d\n", argv[1],errno);
			exit(1);
		}
	}
	else
	{
		printf("Error: Not Enough Arguments!\n");
		exit(1);
	}

	if(fstat(fd, &st) == -1)
	{
		printf("Error: [%s]\n",strerror(errno));
		close(fd);
		exit(1);
	}

	printf("File size:			%lld bytes\n",(long long) st.st_size);
	close(fd);
}




