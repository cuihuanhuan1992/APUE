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
#define wlock(fd, whence, start, len) 	request_lock(fd, F_SETLKW, F_WRLCK, whence, start, len)
#define unlock(fd, whence, start, len) 	request_lock(fd, F_SETLKW, F_UNLCK, whence, start, len)

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

	ret = write(fd, "0", 1);
	if(ret < 0){
		perror("write");
		exit(1);
	}
	
	pid = fork();
	if(pid < 0){
		perror("fork");
		exit(1);
	}else if(pid == 0){
		while(count--){
			wlock(fd, SEEK_SET, 0, 0);
			lseek(fd, 0, SEEK_SET);
			ret = read(fd, buf, 32);
			buf[ret] = 0;
			ret = sprintf(buf, "%d", atoi(buf) + 1);	
			lseek(fd, 0, SEEK_SET);
			write(fd, buf, ret);
			unlock(fd, SEEK_SET, 0, 0);
		}
		exit(0);
	}

	pid = fork();
	if(pid < 0){
		perror("fork");
		exit(1);
	}else if(pid == 0){
		while(count--){
			wlock(fd, SEEK_SET, 0, 0);
			lseek(fd, 0, SEEK_SET);
			ret = read(fd, buf, 32);
			buf[ret] = 0;
			ret = sprintf(buf, "%d", atoi(buf) + 1);	
			lseek(fd, 0, SEEK_SET);
			write(fd, buf, ret);
			unlock(fd, SEEK_SET, 0, 0);
		}
		exit(0);
	}

	while(1){
		ret = wait(NULL);
		if(ret < 0)
			break;
	}
	
	return 0;
}
