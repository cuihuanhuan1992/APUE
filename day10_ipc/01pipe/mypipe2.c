#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd[2];
	char buf[4096];
	int ret, count = 0;
	int len = 65536 - 10;

	//if(pipe(fd) < 0){
	if(pipe2(fd, O_NONBLOCK) < 0){
		perror("pipe");
		exit(1);
	}

	while(len--){
		if((ret = write(fd[1], "a", 1)) < 0){
			perror("write");
			break;
		}
		count += ret;
	}

	printf("pipe write ok %d bytes\n", count);

	if((ret = read(fd[0], buf, 4095)) < 0){
	//if((ret = read(fd[0], buf, 4096)) < 0){
		perror("read");
		exit(1);
	}

	ret = write(fd[1], buf, 30);
	if(ret < 0){
		perror("write again\n");
		exit(1);
	}

	close(fd[0]);
	close(fd[1]);
	
	return 0;
}






