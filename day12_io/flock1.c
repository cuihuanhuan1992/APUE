#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int request_lock(int fd)
{
	struct flock lock = {
		.l_type = F_WRLCK,  
		.l_whence = SEEK_SET,
		.l_start = 0, 
		.l_len = 0, 
	};

	return fcntl(fd, F_SETLK, &lock);
}

int main(void)
{
	pid_t pid;
	int fd;
	int count = 10000;
	char buf[32];
	int ret;

	fd = open("./flock.txt", O_RDWR | O_CREAT | O_TRUNC, 0640);
	if(fd < 0){
		perror("open ./flock.txt");
		exit(1);
	}

	if(request_lock(fd) < 0){
		perror("aleady run");
		exit(1);
	}

	while(1){
		printf("I am runing\n");
		sleep(1);
	}
	
	return 0;
}





