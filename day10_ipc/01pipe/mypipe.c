#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd[2];
	int ret;
	int count = 0;
	int flag;

	if(pipe(fd) < 0){
		perror("pipe");
		exit(1);
	}

	printf("PIPE_BUF = %d\n", (int)fpathconf(fd[0], _PC_PIPE_BUF));

	flag = fcntl(fd[1], F_GETFL, 0);
	if(flag < 0){
		perror("fcntl");
		exit(1);
	}

	ret = fcntl(fd[1], F_SETFL, flag | O_NONBLOCK);
	if(ret < 0){
		perror("fcntl");
		exit(1);
	}

	while(1){
		ret = write(fd[1], "a", 1);
		if(ret < 0){
			perror("write");
			break;
		}
		count += ret;
	}	

	printf("len of pipe : %d\n", count);

	close(fd[0]);
	close(fd[1]);
	
	return 0;
}
