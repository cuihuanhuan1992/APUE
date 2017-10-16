#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
	unsigned int *v;
	pid_t pid;

	v = mmap(0, sizeof(int), PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if(v == MAP_FAILED){
		perror("mmap error");
		exit(1);
	}

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
















