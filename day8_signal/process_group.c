#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void do_int(int no)
{
	printf("pid:%d\n", getpid());
}

int main(void)
{
	pid_t pid;

	signal(SIGINT, do_int);

	pid = fork();
	if(pid < 0){
		perror("fork");
	}else if(pid == 0){
		sleep(100);
		exit(0);
	}

	sleep(100);

	return 0;
}
