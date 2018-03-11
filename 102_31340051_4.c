/* 
	C program that uses two processes (not threads) to
	sort the regular files in a directory and the sub
	directories based on file sizes.

	Format of the command for running the program:

	./your_program pathname_of_directory

	Program should be concerned with all the regular files.
	Only checks the sizes of regular files. It does not check
	the sizes of directories or symbolic links. Do not include 
	the sizes of directories or symbolic links in the results.

	Print out the results on the screen in text format, one line 
	for each file with file size followed by a tabular symbol ('t') 
	and the file pathname.

	The parent process needs to use a structure to contain the 
	information of each file which is dynamically allocated when
	the information is received, and use a data structure, such as 
	linked-lists, binary trees, or a hash table, to orgranize the 
	structures of files.

	The parent process needs to sort the files incrementally.
	It maintains a sorted list. When information arrives from
	the child, it updates the list. In this way, the parent process 
	can provide a sorted list immediately after the child process 
	finishes scanning --- the processes running in parallel.
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#include <sys/syscall.h>

#define BUF_SIZE 1024

int cl_direcDes(char *argv[]);

struct linux_dirent {
	long			d_ino;		// Inode number
	off_t			d_off; 		// Offset to next dirent
	unsigned short	d_reclen;	// Length of the current dirent
	char			d_name[];	// Filename 
};
/*
struct stat {
	dev_t st_dev;			//ID of device
	ino_t st_ino;			//Inode number
	mode_t st_mode;			//File Type
	nlink_t st_nlink;		//Number of hard links
	uid_t st_uid;			//User ID of owner
	gid_t st_gid;			//Group ID of owner
	dev_t st_rdev;			//Device ID (if special file)
	off_t st_size;			//Total size in bytes
	blksize_t st_blksize;	//Block size for filesystem I/O
	blkcnt_t st_blocks; 	//Number of 512B blocks allocated
};
*/

extern int errno;
int main(int argc, char *argv[])
{
	int direct_Descriptor, numbytes_read;
	char buf[BUF_SIZE];
	struct linux_dirent *dirEntry;

	direct_Descriptor = cl_direcDes(argv);
	
	/* 
		'direct_Descriptor' is the file descriptor for the pathname of the directory
	*/
	// int fstat(int fd, struct stat *statbuf);
	
	int b_position;

	//for( ; ; )
	//{
		numbytes_read = syscall(SYS_getdents, direct_Descriptor, buf, BUF_SIZE );
		if(numbytes_read == -1) // Error
		{
			printf("Error! errno = %d", errno);
			exit(1);
		}
		if(numbytes_read == 0) // EOF
		{
			exit(0);
		}

		for	(b_position = 0; b_position < numbytes_read; )
		{
			dirEntry = (struct linux_dirent *)(buf + b_position);
			printf("%s\n", dirEntry->d_name);
			b_position += dirEntry->d_reclen;
		}	
	//}
	return 0;

}

int cl_direcDes(char *argv[])
{
	int fd;
	fd = open(argv[1], O_RDONLY | O_DIRECTORY);
	if(fd == -1)
	{
		printf("Error: Cannot find the directory at %s , errno = %d\n",argv[1], errno);
		exit(1);
	}
	
	return fd;
}