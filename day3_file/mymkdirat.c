#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int ret;
	int fd;

	fd = open("hello", O_DIRECTORY);
	if(fd < 0){
		perror("open hello");
		exit(1);
	}

	ret = mkdirat(fd, argv[1], 0775);
	if(ret < 0){
		perror("mkdir error");
		exit(1);
	}

	close(fd);
	
	return 0;
}
