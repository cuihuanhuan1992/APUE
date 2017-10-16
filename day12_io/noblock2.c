#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>

int main(void)
{
	char buf[1024];
	int ret;
	long flags;
	pid_t pid;
	int pfd[2];

 	ret = pipe(pfd);
 	//ret = pipe2(pfd, O_NONBLOCK);
	if(ret < 0){
		perror("pipe");
		exit(1);
	}

	flags = fcntl(pfd[0], F_GETFL, 0);
	if(flags < 0){
		perror("fcntl0");
		exit(1);
	}

	ret = fcntl(pfd[0], F_SETFL, flags | O_NONBLOCK);
	if(ret < 0){
		perror("fcntl1");
		exit(1);
	}

	pid = fork();
	if(pid < 0){
		perror("fork");
		exit(1);
	}else if(pid == 0){
		close(pfd[0]);		
		while(1){
			ret = write(pfd[1], "helloworld\n", 11);
			if(ret < 0){
				perror("write child");
				exit(1);
			}
			sleep(2);
		}
		close(pfd[1]);
		exit(0);
	}

	close(pfd[1]);

	while(1){
		again:
		ret = read(pfd[0], buf, 1024);
		if(ret < 0){
			fprintf(stderr, "read return %d\n", errno);
			sleep(1);
			goto again;
		}

		ret = write(1, buf, ret);	
		if(ret < 0){
			perror("write error");
			exit(1);
		}
	}

	close(pfd[0]);

	return 0;
}





