#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
	int fd;
	int newfd;

	fd = open("dup.txt", O_RDWR);
	if(fd < 0){
		perror("open dup.txt");
		exit(1);
	}

	//close(1);
	////如果在这里被打断，另外一个线程可能打开一个文件，这样就dup的返回值就不再是1
	//newfd = dup(fd);	

	//下面是一个原子操作
	newfd = dup2(fd, 1);

	close(fd);

	printf("helloworld\n");

	return 0;
}
