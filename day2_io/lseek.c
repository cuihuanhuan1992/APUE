#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
	int fd;

	fd = open("./text.txt", O_RDWR | O_CREAT | O_TRUNC, 0660);
	if(fd < 0){
		perror("open ./text.txt");
		exit(1);	
	}

	write(fd, "hello", 5);
	/* if error */
	
	lseek(fd, 0, SEEK_SET);
	/* if error */

	write(fd, "aa", 2);
	/* if error */

	close(fd);
	
	return 0;
}
