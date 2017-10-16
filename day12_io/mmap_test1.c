#include <stdlib.h>
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

	fd = open("./plus.plugin", O_RDONLY);
	if(fd < 0){
		perror("open");
		exit(1);
	}

	ret = fstat(fd, &s);
	if(ret < 0){
		perror("stat");
		exit(1);
	}

	v = mmap(0, s.st_size, PROT_READ | PROT_EXEC, MAP_PRIVATE, fd, 0);
	if(v == MAP_FAILED){
		perror("mmap error");
		exit(1);
	}

	h = (void *)(v + 0x55c);
	
	printf("%d\n", h(3, 7));	

	munmap(v, s.st_size);
	close(fd);

	return 0;
}






