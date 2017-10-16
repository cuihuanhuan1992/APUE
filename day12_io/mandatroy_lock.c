#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int request_lock(int fd, int cmd, short type, short whence, off_t start, off_t len)
{
	struct flock lock = {
		.l_type = type,  
		.l_whence = whence,
		.l_start = start, 
		.l_len = len, 
	};

	return fcntl(fd, cmd, &lock);
}

#define rlock(fd, whence, start, len) 	request_lock(fd, F_SETLKW, F_RDLCK, whence, start, len)
#define wlock(fd, whence, start, len) 	request_lock(fd, F_SETLK, F_WRLCK, whence, start, len)
#define unlock(fd, whence, start, len) 	request_lock(fd, F_SETLKW, F_UNLCK, whence, start, len)

int main(void)
{
	pid_t pid;
	int fd;
	int count = 10000;
	char buf[32];
	long flags;
	int ret;
	struct stat s;

	fd = open("./flock.txt", O_RDWR | O_CREAT | O_TRUNC, 0640);
	if(fd < 0){
		perror("open ./flock.txt");
		exit(1);
	}

	ret = fstat(fd, &s);
	if(ret < 0){
		perror("fstat");
		exit(1);
	}

	//打开设置组ID位，关闭组执行位
	s.st_mode |= S_ISGID;
	s.st_mode &=~S_IXGRP;
	ret = fchmod(fd, s.st_mode);	
	if(ret < 0){
		perror("fchmod");
		exit(1);
	}

	pid = fork();
	if(pid < 0){
		perror("fork");
		exit(1);
	}else if(pid == 0){
		wlock(fd, SEEK_SET, 0, 0);
		sleep(5);
		unlock(fd, SEEK_SET, 0, 0);
		exit(0);
	}

	sleep(1);
	
	ret = fcntl(fd, F_GETFD, &flags);
	if(ret < 0){
		perror("fcntl0");
		exit(1);
	}

	ret = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
	if(ret < 0){
		perror("fcntl1");
		exit(1);
	}

	ret = wlock(fd, SEEK_SET, 0, 0);
	if(ret < 0){
		fprintf(stderr, "father lock error\n");
	}else{
		printf("father lock\n");
	}

	ret = write(fd, "father hehe error\n", 20);
	if(ret < 0){
		perror("write");
		exit(1);
	}

	printf("write ok\n");

	//unlock(fd, SEEK_SET, 0, 0);

	wait(NULL);
	
	return 0;
}
