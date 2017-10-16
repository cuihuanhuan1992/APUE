#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

extern char **environ;

int main(int argc, char *argv[])
{
	printf("%d\n", (int)geteuid());

	setenv("PS1", "root@uplooking \\W# ", 1);

	execl("/bin/bash", "-sh", NULL);
	
	perror("execl");	

	return 0;
}
