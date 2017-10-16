#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
	int fd;
	char buf[32];
	int ret;
	struct stat stat;
	struct timespec timespec[2];

	fd = open("futimens.txt", O_RDWR | O_CREAT, 0666);
	if(fd < 0){	
		perror("open");
		exit(1);
	}

	ret = fstat(fd, &stat);
	if(ret < 0){
		perror("fstat");
		exit(1);
	}

	printf("last access time:%lu\nlast modify time:%lu\nlast change time:%lu\n", stat.st_atime, stat.st_mtime, stat.st_ctime);
	printf("last access time:%s\n", ctime_r(&stat.st_atime, buf));
	printf("last modify time:%s\n", ctime_r(&stat.st_mtime, buf));
	printf("last change time:%s\n", ctime_r(&stat.st_ctime, buf));

	ret = read(fd, buf, 10);
	if(ret < 0){
		perror("read");
		exit(1);
	}	

	ret = write(fd, buf, 10);
	if(ret < 0){
		perror("write");
		exit(1);
	}	

	timespec[0].tv_sec = stat.st_atime;
	timespec[1].tv_sec = stat.st_mtime;	

	ret = futimens(fd, timespec);
	if(ret < 0){
		perror("futimens");
		exit(1);
	}

	close(fd);

	return 0;
}

