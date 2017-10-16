#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv)
{
	int fd;
	int i;

	fd = open("dup.txt", O_WRONLY);
	if(fd < 0){
		perror("open dup.txt");
		exit(1);
	}

	for(i = 0; i < 1000; i++){
		write(fd, "nihao", 5);
		if(argc != 1)
			fsync(fd);
	}
		/* if error */

	close(fd);

	return 0;
}
