#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd;
	int ret;
	struct stat s;
	char *v;
	int (*h)(int a, int b);

	fd = open("./readwrite.txt", O_RDWR);
	if(fd < 0){
		perror("open");
		exit(1);
	}

	ret = fstat(fd, &s);
	if(ret < 0){
		perror("stat");
		exit(1);
	}

	v = mmap(0, s.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	//v = mmap(0, s.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	if(v == MAP_FAILED){
		perror("mmap error");
		exit(1);
	}

	write(1, v, s.st_size);

	memset(v, 0, s.st_size);

	munmap(v, s.st_size);
	close(fd);

	return 0;
}






