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
#include <string.h>

#define BUF_SIZE 1024
#define MAX_FILENAME 100

char * cl_direcDes(char *argv[]);
struct stat dir_processor(char *pathname);


struct linux_dirent {
	long			d_ino;		// Inode number
	off_t			d_off; 		// Offset to next dirent
	unsigned short	d_reclen;	// Length of the current dirent
	char			d_name[];	// Filename 
 //	char			d_type;		// Might not be accepted in different systems. Therefore we will not use it for this program.
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

int b_position;
static struct stat statStruct;

extern int errno;
int main(int argc, char *argv[])
{
	pid_t pid;
	int pipefd[2];
	int ret;
	//char *buffed = malloc(MAX_FILENAME);
	struct stat main_Stat;
/*
	ret = pipe(pipefd);
	if(ret == -1)
	{
		printf("Error: Couldn't set up pipe! errno = %d", errno);
		exit(1);
	}

	pid = fork();
	if(pid == (pid_t)0) // child process
	{
		// closing the input side of pipe
		close(pipefd[0]);



	}

	else // Parent process
	{
		// closing the output side of pipe
		close(pipefd[1]);
	}
	
*/	
	// buffed = dir_processor(cl_direcDes(argv));
	main_Stat = dir_processor(cl_direcDes(argv));
	printf("%d\n", (int) main_Stat.st_size);
	// int fstat(int fd, struct stat *statbuf);
	
	return 0;

}

char * cl_direcDes(char *argv[])
{
	struct stat statter;
	int stat_Result = stat(argv[1], &statter);
	if(S_ISREG(statter.st_mode))
	{
		printf("Error: Path '- %s -' leads to a file.\n",argv[1]);
		exit(1);
	}
	if(S_ISDIR(statter.st_mode))
		return argv[1];
}

struct stat dir_processor(char * pathname)
{
	int fd, nbytes_read; 
	b_position = 0;
	struct linux_dirent *dirEntry;
	char fileName[MAX_FILENAME];
	char buf[BUF_SIZE]; 
	char *pathname_buffer = malloc(MAX_FILENAME);

	fd = open(pathname, O_RDONLY);
	
	nbytes_read = syscall(SYS_getdents, fd, buf, BUF_SIZE); // using getdents
	//printf("%s 			%d\n\n", pathname, fd);
	dirEntry = (struct linux_dirent*)(buf + b_position); // casting buf + b_position to dirEntry 
														 // in order to move to the next file
	if(nbytes_read == -1)
	{
		printf("Error: errno = %d, %s\n", errno, strerror(errno));
		exit(1);
	}
	if(nbytes_read == 0) // EOF
	{
		printf("nbytes_read = 0: EOF");
		exit(1);//break;
	}
	// Pass original pathname + file to pathname_buffer
	sprintf(pathname_buffer, "%s/%s", pathname, (char *) dirEntry->d_name);
	
	int status = stat(pathname_buffer, &statStruct);

	if(S_ISREG(statStruct.st_mode)) // If the current item is a file
	{
		printf("%s\n", pathname_buffer);
		return statStruct;	       // Return the struct( or more likely return a pointer to the struct)
	}

	else if(S_ISDIR(statStruct.st_mode))
		dir_processor(pathname_buffer);

	//printf("%s\n", pathname_buffer);
	//return pathname_buffer;
	//b_position += dirEntry->d_reclen;
	//dirEntry = (struct linux_dirent*)(buf + b_position);
	// Left off passing struct value through recursive function/check S_ISREG
}

