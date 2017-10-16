#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int fd;

	fd = open("append.txt", O_WRONLY | O_CREAT | O_APPEND, 0660);
	if(fd < 0){
		perror("open append.txt");
		exit(1);
	}

//	lseek(fd, 0, SEEK_END);
//	sleep(2);
	write(fd, argv[1], atoi(argv[2]));

	close(fd);

	return 0;
}
