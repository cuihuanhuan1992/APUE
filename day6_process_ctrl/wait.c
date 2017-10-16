#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
	pid_t pid;
	int status;

	pid = fork();
	if(pid < 0){
		perror("fork error");
		exit(1);
	}else if(pid == 0){
		printf("i am child\n");	
		sleep(10);
		exit(10);
	}

	pid = fork();
	if(pid < 0){
		perror("fork error");
		exit(1);
	}else if(pid == 0){
		printf("i am another child\n");
		sleep(5);
		abort();
	}

	while(1){
		pid = wait(&status);	
		if(pid < 0)
			break;

		if(WIFEXITED(status)){
			printf("child return:%d\n", WEXITSTATUS(status));
		}

		if(WIFSIGNALED(status)){
			printf("terminaled by signal %d\n", WTERMSIG(status));
		}
	}

	return 0;
}












