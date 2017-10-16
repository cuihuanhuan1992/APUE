#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd[2];
	int fd1[2];
	char buf[32];
	int ret;
	pid_t pid;

	if(pipe(fd) < 0){
		perror("pipe");
		exit(1);
	}

	if(pipe(fd1) < 0){
		perror("pipe");
		exit(1);
	}


	/*
	写：fd[1]fu----------------->fd[0]zi
	写：fd1[1]zi----------------->fd1[0]fu

	*/

	pid = fork();	
	if(pid < 0){
		perror("fork");
		exit(1);
	}else if(pid == 0){
		close(fd[1]);
		close(fd1[0]);
		dup2(fd[0], 0);
		dup2(fd1[1], 1);
		execl("chario", "chario", NULL);
		perror("execl");
		exit(1);
	}

	close(fd[0]);
	close(fd1[1]);

	ret = write(fd[1], "nihao wo shi haha\n", 18);
	if(ret < 0){
		perror("father write");
		exit(1);
	}

	wait(NULL);
	
	ret = read(fd1[0], buf, sizeof(buf));
	buf[ret] = 0;
	printf("from chario %s\n", buf);
	
	close(fd[1]);
	close(fd1[0]);
	
	return 0;
}
