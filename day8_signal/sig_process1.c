#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void process_state(int no)
{
	int state;

	printf("father id %d\n", (int)getpid());

	wait(&state);

	if(WIFEXITED(state))
		printf("exit value %d\n", WEXITSTATUS(state));
}

int main(void)
{
	pid_t pid;
	int count = 10;

	signal(SIGCHLD, process_state);
	signal(SIGINT, process_state);
	/*if error*/

	pid = fork();
	if(pid < 0){
		perror("fork error");
		exit(1);
	}else if(pid == 0){

		pid = fork();
		if(pid == 0)
			exit(20);

		while(count--){
			printf("i am child\n");	
			sleep(1);
		}

		exit(10);
	}

	//pid = wait(NULL);
	//if(pid < 0)
	//	perror("wait error");

	sleep(10);

	while(count--){
		printf("i am father\n");
		sleep(1);
	}	

	return 0;
}












