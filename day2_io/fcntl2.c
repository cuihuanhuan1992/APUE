#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv)
{
	int fd;
	int flag;
	int ret;
	

	fd = open("dup.txt", O_RDWR /*| O_APPEND*/);
	if(fd < 0){
		perror("open dup.txt");
		exit(1);
	}

	flag = fcntl(fd, F_GETFL, 0);
	/* if error */

	switch(flag & O_ACCMODE){
		case O_WRONLY:
			printf("only write\n");
			break;
		case O_RDONLY:
			printf("only read\n");
			break;
		case O_RDWR:
			printf("read write\n");
			break;
		default:
			printf("unknow mode\n");
			break;
	}

	if(flag & O_APPEND)
		printf("append1\n");

	flag |= O_APPEND;

	ret = fcntl(fd, F_SETFL, flag);
	if(ret < 0){
		perror("fcntl set");
		exit(1);
	}

	if(flag & O_APPEND)
		printf("append2\n");

	ret = write(fd, "nihao", 5);
	if(ret < 0){
		perror("write");
		exit(1);
	}

	close(fd);

	return 0;
}














