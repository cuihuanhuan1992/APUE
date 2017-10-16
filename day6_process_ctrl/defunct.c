#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
	pid_t pid;
	int count = 10;

	pid = fork();
	if(pid < 0){
		perror("fork error");
		exit(1);
	}else if(pid == 0){
		pid = fork();
		if(pid > 0){
			sleep(2);
			exit(0);
		}

		while(count--){
			printf("i am child\n");	
			sleep(1);
		}

		exit(10);
	}

	pid = wait(NULL);
	if(pid < 0)
		perror("wait error");

	while(count--){
		printf("i am father\n");
		sleep(1);
	}	

	return 0;
}












