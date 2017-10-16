#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>

int main(int argc, char *argv[])
{
	pid_t pid;
	int fd, fd1;

	fd = open("./pipe.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);
	if(fd < 0){
		perror("open");
		exit(1);
	}

	fd1 = open("./pipe1.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);
	if(fd1 < 0){
		perror("open");
		exit(1);
	}

	pid = fork();
	if(pid < 0){
		perror("main fork");
		exit(1);
	}else if(pid == 0){
		pid = fork();
		if(pid < 0){
			perror("fork1");
			exit(1);
		}else if(pid == 0){
			dup2(fd, 1);	
			close(fd);
			close(fd1);
			execl(argv[1], argv[1], NULL);
		}

		wait(NULL);

		pid = fork();
		if(pid < 0){
			perror("fork1");
			exit(1);
		}else if(pid == 0){
			lseek(fd, 0, SEEK_SET);
			dup2(fd, 0);	
			dup2(fd1, 1);	
			close(fd);
			close(fd1);
			execl(argv[3], argv[3], NULL);
		}

		wait(NULL);
		lseek(fd1, 0, SEEK_SET);
		dup2(fd1, 0);
		close(fd);
		close(fd1);
		
		execl(argv[5], argv[5], NULL);	
	}

	close(fd);

	wait(NULL);

	return 0;
}
