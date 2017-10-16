#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd[2];
	char buf[32];
	int ret;
	pid_t pid;

	if(pipe(fd) < 0){
		perror("pipe");
		exit(1);
	}

	pid = fork();	
	if(pid < 0){
		perror("fork");
		exit(1);
	}else if(pid == 0){
		close(fd[0]);
		write(fd[1], "child", 5);
		/*if error*/
		close(fd[1]);
		exit(0);
	}

	close(fd[1]);

	wait(NULL);
	
	ret = read(fd[0], buf, sizeof(buf));
	buf[ret] = 0;
	printf("from child %s\n", buf);
	
	close(fd[0]);
	
	return 0;
}
