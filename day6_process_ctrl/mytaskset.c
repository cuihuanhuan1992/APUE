
#define _GNU_SOURCE

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <time.h>
#include <sched.h>

int main(int argc, char *argv[])
{
	pid_t pid;
	cpu_set_t mask;

	pid = fork();
	if(pid < 0){
		perror("fork");
		exit(1);
	}else if(pid == 0){
		CPU_ZERO(&mask);
		CPU_SET(atoi(argv[1]), &mask);
		sched_setaffinity(0, sizeof(cpu_set_t), &mask);
		execv(argv[2], argv + 2);
		perror("execv");
		exit(1);
	}

	wait(NULL);

	return 0;
}



















