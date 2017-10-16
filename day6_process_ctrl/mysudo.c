#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

extern char **environ;

int main(int argc, char *argv[])
{
	execvp(argv[1], argv + 1);
	
	perror("execl");	

	return 0;
}
