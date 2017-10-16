#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

#define BUFSIZE 10

int main(int argc, char *argv[])
{
	char buf[BUFSIZE];
	int ret = 1;
	int fd;

	ret = read(STDIN_FILENO, buf, BUFSIZE);
	if(ret < 0){
		perror("read stdin");
		exit(1);
	}

	printf("read ok:%d %d\n", ret, buf[ret - 1]);

	ret = write(STDOUT_FILENO, buf, ret);
	if(ret < 0){
		perror("read stdin");
		exit(1);
	}

	close(STDOUT_FILENO);

	fd = open("./test.txt", O_WRONLY | O_TRUNC | O_CREAT, 0660);
	if(fd < 0){
		perror("open test.txt");
		exit(1);
	}

	printf("hello nihao uplooking\n");

	fflush(stdout);

	close(fd);
	
	return 0;
}














