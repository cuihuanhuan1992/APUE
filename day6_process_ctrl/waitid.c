#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
	pid_t pid;
	int status;
	siginfo_t info;

	pid = fork();
	if(pid < 0){
		perror("fork error");
		exit(1);
	}else if(pid == 0){
		printf("i am child %d\n", getpid());	
		sleep(10);
		exit(10);
	}

	pid = fork();
	if(pid < 0){
		perror("fork error");
		exit(1);
	}else if(pid == 0){
		printf("i am another child %d\n", getpid());
		sleep(5);
		abort();
	}

	while(1){
		//pid = waitid(P_ALL, 0, &info, WEXITED | WNOHANG);	
		pid = waitid(P_ALL, 0, &info, WEXITED);	
		if(pid < 0)
			break;

		printf("one child terminal %d\n", info.si_pid);
	}

	return 0;
}












