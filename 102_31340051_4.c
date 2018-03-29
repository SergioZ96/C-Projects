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

// Credit to Gabriel Barros for clean code

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <string.h>

#define BUF_SIZE 1024
#define MAX_FILENAME 500

char * cl_direc(char *argv[]);
void dir_processor(char * pathname, FILE * writer);
void fileread_info(FILE * reader, char * filename, int size);
void sort(int size, char *f_name);


struct linux_dirent {
	long			d_ino;		// Inode number
	off_t			d_off; 		// Offset to next dirent
	unsigned short	d_reclen;	// Length of the current dirent
	char			d_name[];	// Filename 
 //	char			d_type;		// Might not be accepted in different systems. Therefore we will not use it for this program.
};

 struct file_info{
	off_t file_size;
	char file_name[MAX_FILENAME];
};

struct Node
{
	int filesize;
	char *filename;
	struct Node *next;
};

struct Node *head = NULL;

//int b_position;

extern int errno;
int main(int argc, char *argv[])
{
	pid_t pid;
	int pipefd[2], ret;
	char *arg_path = cl_direc(argv);

	ret = pipe(pipefd);
	if(ret == -1)
	{
		printf("Error: Couldn't set up pipe! Reason: %s\n", strerror(errno));
		exit(1);
	}

	FILE *reader = fdopen(pipefd[0], "r");
	FILE *writer = fdopen(pipefd[1], "w");

	pid = fork();
	if(pid == (pid_t)0) // child process
	{
		// closing the input side of pipe
		close(pipefd[0]);
		dir_processor(arg_path, writer);

	}

	else // Parent process
	{
		struct file_info file;

		// closing the output side of pipe
		close(pipefd[1]);
		fileread_info(reader, file.file_name, file.file_size);

		struct Node *node = head;
		while(node != NULL)
		{
			printf("%d\t%s\n", node->filesize, node->filename);
			node = node->next;
		}
		
		wait(NULL);
	}
	
	
	return 0;

}

char * cl_direc(char *argv[])
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


void dir_processor(char * path, FILE * writer)
{
	int fd, nbytes, b_position; 
	char fileName[MAX_FILENAME];
	char buf[BUF_SIZE]; 
	char *pathname_buffer = malloc(MAX_FILENAME);

	struct linux_dirent *dirEntry;
	struct stat statStruct;

	fd = open(path, O_RDONLY);
	if(fd == -1)
	{
		printf("Error: Cannot open pathname %, errno: %s\n", path, strerror(errno));
		exit(1);
	}
	nbytes = syscall(SYS_getdents, fd, buf, BUF_SIZE); 
	
	for(b_position = 0;b_position < nbytes;)
	{
		// using getdents

		dirEntry = (struct linux_dirent*)(buf + b_position);
											
		if(nbytes == -1)
		{
			printf("Error: Cannot retrieve directory entries. Reason: %s\n", strerror(errno));
			exit(1);
		}
		if(nbytes == 0) // EOF
		{
			break;
		}

		if(strcmp(dirEntry->d_name,".") != 0 &&
			strcmp(dirEntry->d_name,"..") != 0)
		{
			
				sprintf(pathname_buffer, "%s/%s", path, (char *) dirEntry->d_name);	
				stat(pathname_buffer, &statStruct);
				

				if(S_ISREG(statStruct.st_mode)) // If the current item is a file
				{
					fprintf(writer, "%s , %d\n", pathname_buffer, statStruct.st_mode);
					fflush(writer);
				}

				else if(S_ISDIR(statStruct.st_mode))
					dir_processor(pathname_buffer, writer);
				
		}

		b_position += dirEntry->d_reclen;
		
	}

	close(fd);
}

void fileread_info(FILE * reader, char * filename, int size)
{
	while(fscanf(reader, "%s , %d\n", filename, &size) == 2)
	{
		char *f_name = malloc(strlen(filename)+1);
		strcpy(f_name, filename);

		// add/sort data sent to linked list
		sort(size,f_name);
	}
}

void sort(int size, char *f_name)
{
	struct Node *temp = head;
	struct Node *prev = NULL;
	struct Node *node;

	node = (struct Node*) malloc(sizeof(struct Node));
	node->filesize = size;
	node->filename = f_name;
	node->next = NULL;

	if(temp == NULL)
	{
		node->next = NULL;
		head = node;
		return;
	}

	if(size < temp->filesize)
	{
		node->next = head;
		head = node;
		return;
	}

	else
	{
		while(temp  != NULL)
		{
			if(size > temp->filesize || size == temp->filesize)
			{
				prev = temp;
				temp = temp->next;
				continue;
			}
			else
			{
				prev->next = node;
				node->next = temp;
				return;
			}
		}
		prev->next = node;
	}
}