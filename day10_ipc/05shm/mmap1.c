#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
	int fd;
	unsigned int *v;
	pid_t pid;

	fd = open("/dev/zero", O_RDWR);
	if(fd < 0){
		perror("open /dev/zero");
		exit(1);
	}

	v = mmap(0, sizeof(int), PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
	if(v == MAP_FAILED){
		perror("mmap error");
		exit(1);
	}

	close(fd);

	pid = fork();
	if(pid < 0){
		perror("fork");
		exit(1);
	}else if(pid == 0){
		*v = 100;
		exit(0);
	}	

	wait(NULL);

	printf("*v = %d\n", *v);		

	munmap(v, sizeof(int));

	return 0;
}
















