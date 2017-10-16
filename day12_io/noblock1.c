#include <stdio.h>
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

	pid = fork();
	if(pid < 0){
		perror("fork");
		exit(1);
	}else if(pid == 0){
		close(pfd[0]);		
		ret = write(pfd[1], "helloworld\n", 11);
		if(ret < 0){
			perror("write child");
			exit(1);
		}
		exit(0);
	}

	close(pfd[1]);

	//sleep(10);

	ret = read(pfd[0], buf, 1024);
	if(ret < 0){
		perror("read father");
		exit(1);
	}

	buf[ret] = 0;

	ret = write(1, buf, ret);	
	if(ret < 0){
		perror("write error");
		exit(1);
	}

	return 0;
}





