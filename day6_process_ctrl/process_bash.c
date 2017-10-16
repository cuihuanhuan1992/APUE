#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int i;
	pid_t pid;

	pid = fork();
	if(pid < 0){
		perror("fork");
		exit(1);
	}else if(pid == 0){
		execl("/root/test/test.sh", "one", "two", "three", NULL);
		perror("execl");
	}

	wait(NULL);

	return 0;
}
