#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>

int main(void)
{
	int ret;
	int fd[2];
	char buf[1024];
	pid_t pid;

	ret = socketpair(AF_UNIX, SOCK_STREAM, 0, fd);
	if(ret < 0){
		perror("socketpair");
		exit(1);
	}

	pid = fork();	
	if(pid < 0){
		perror("fork");
		exit(1);
	}else if(pid == 0){
		close(fd[0]);
		write(fd[1], "hello", 6);
		ret = read(fd[1], buf, sizeof(buf));
		write(1, buf, ret);
		close(fd[1]);
		exit(0);
	}else{
		close(fd[1]);
		ret = read(fd[0], buf, 2);
		write(1, buf, ret);
		printf("\n");
		ret = read(fd[0], buf, sizeof(buf));
		write(1, buf, ret);
		sleep(2);
		write(fd[0], "world", 6);
	}

	close(fd[0]);

	return 0;
}
