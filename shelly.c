// Implementing a mini Linux Shell with basic functionality for Interactive and Batch Mode Processing

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define READ_BUFSIZE 1024
#define INTER_ACTIVE 1
#define BATCH 2

extern int errno;

char *read_shell_line(void);
char **line_parser(char* line);
void shell_loop(void);

int main(int argc, char *argv[])
{
	if(argc == INTER_ACTIVE)
	{
		shell_loop();
	}
	if(argc == BATCH)
	{

	}
}

char *read_shell_line(void)
{
	int pos = 0, bufsize = READ_BUFSIZE;
	char *buffer = malloc(sizeof(char) * bufsize);
	int c;

	
	fgets( buffer, bufsize, stdin);
	return buffer;
}

char **line_parser(char *line)
{
	char *string, *parsed_arg;
	
}

void shell_loop(void)
{
	while(1)
	{
		printf("prompt> ");
		char *line = read_shell_line();
	}

}
