#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
	int fd;
	int fd_hole;
	int ret;
	int count = 40960;

	fd_hole = open("./file.hole", O_CREAT | O_WRONLY | O_TRUNC, 0660);
	if(fd < 0){
		perror("open ./file.hole");
		exit(1);
	}

	ret = write(fd_hole, "nihao", 5);
	if(ret < 0){
		perror("write file.hole");
		exit(1);
	}	

	ret = lseek(fd_hole, 40960, SEEK_END);
	if(ret < 0){
		perror("lseek file.hole");
		exit(1);
	}

	ret = write(fd_hole, "hello", 5);
	if(ret < 0){
		perror("write file.hole");
		exit(1);
	}

	//-------------------------------------------------------------------

	fd = open("./file.txt", O_CREAT | O_WRONLY | O_TRUNC, 0660);
	if(fd < 0){
		perror("open ./file.txt");
		exit(1);
	}

	ret = write(fd, "nihao", 5);
	if(ret < 0){
		perror("write test.txt");
		exit(1);
	}	

	while(count){
		ret = write(fd, "0000", 4);
		if(ret < 0){
			perror("write file.txt");	
			exit(1);
		}

		count -= ret;
	}

	ret = write(fd, "hello", 5);
	if(ret < 0){
		perror("write file.txt");
		exit(1);
	}

	close(fd_hole);
	close(fd);

	return 0;
}

















