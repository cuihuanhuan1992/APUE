#include <stdio.h>
#include <sys/uio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <unistd.h>

#define IO_NUM	4

int main(void)
{
	int fd, fd1;
	int ret;

	struct iovec io[IO_NUM];
	char buf[4][10];

	io[0].iov_base = "hello";
	io[0].iov_len = 5;
		
	io[1].iov_base = "world";
	io[1].iov_len = 5;

	io[2].iov_base = "nihao";
	io[2].iov_len = 5;

	io[3].iov_base = "123456789\n";
	io[3].iov_len = 10;

	fd = open("readwrite.txt", O_RDWR | O_CREAT | O_TRUNC, 0660);
	if(fd < 0){
		perror("open readwrite");
		exit(1);
	}

	ret = writev(fd, io, IO_NUM);
	if(ret < 0){
		perror("writev");
		exit(1);
	}

	fd1 = open("./aio.txt", O_RDWR);
	if(fd1 < 0){
		perror("open service");
		exit(1);
	}

	io[0].iov_base = buf[0];
        io[0].iov_len = 10;
	io[1].iov_base = buf[1];
        io[1].iov_len = 10;
	io[2].iov_base = buf[2];
        io[2].iov_len = 10;
	io[3].iov_base = buf[3];
        io[3].iov_len = 10;
	
	ret = readv(fd1, io, IO_NUM);
	if(ret < 0){
		perror("readv");
		exit(1);
	}

	ret = writev(fd, io, IO_NUM);
	if(ret < 0){
		perror("readv");
		exit(1);
	}

	printf("ret = %d\n", ret);

	close(fd1);
	close(fd);

	return 0;
}




