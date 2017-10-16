#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>

static int fd[2];
static int fd1[2];

/*
child write fd[1]--------------father read fd[0]
father write fd1[1]--------------child read fd1[0]
*/

int start(void)
{
	if(pipe(fd) < 0 || pipe(fd1) < 0)
		return -1;

	return 0;
}

void end(void)
{
	close(fd[0]);
	close(fd[1]);
	close(fd1[0]);
	close(fd1[1]);
}

int wait_father(void)
{
	char buf[2];

	if(read(fd1[0], buf, 1) < 0){
		perror("wait father");
		return -1;
	}
	
	if(buf[0] != 'f'){
		fprintf(stderr, "wait father error\n");
		exit(0);
	}

	return 0;
}

int wait_child(void)
{
	char buf[2];

	if(read(fd[0], buf, 1) < 0){
		perror("wait child");
		return -1;
	}

	if(buf[0] != 'c'){
		fprintf(stderr, "wait child error\n");
		exit(0);
	}

	return 0;
}

int tell_father(void)
{
	if(write(fd[1], "c", 1) < 0){
		perror("tell father");
		return -1;
	}
}

int tell_child(void)
{
	if(write(fd1[1], "f", 1) < 0){
		perror("tell child");
		return -1;
	}

	return 0;
}


int main(void)
{
	int ret;
	pid_t pid;

	start();

	pid = fork();	
	if(pid < 0){
		perror("fork");
		exit(1);
	}else if(pid == 0){
		while(1){
			wait_father();
			printf("child say:father comming\n");
			sleep(2);
			tell_father();
		}
	}

	while(1){
		tell_child();
		wait_child();
		printf("father say:child comming\n");
		sleep(1);
	}

	end();

	return 0;
}
