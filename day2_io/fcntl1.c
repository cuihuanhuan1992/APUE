#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

extern char **environ;

int main(int argc, char *argv)
{
	int fd;
	int i;
	pid_t pid;

	fd = open("dup.txt", O_WRONLY);
	if(fd < 0){
		perror("open dup.txt");
		exit(1);
	}

	dup2(fd, 1);

	//fcntl(1, F_SETFD, FD_CLOEXEC);	

	close(fd);

	printf("hello nihao\n");

	execl("/bin/ls", "ls", NULL);

	return 0;
}
