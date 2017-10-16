#include <stdio.h>
#include <stdlib.h>
#include <aio.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int i;
	int ret;
	int num;
	unsigned long flags;
	int fifo;
	char *pfifo = "/tmp/myfifo0";

	fifo = open(pfifo, O_RDONLY | O_NONBLOCK);
	if(fifo < 0){
		perror("open fifo");
		exit(1);
	}

	flags = fcntl(fifo, F_GETFL, 0);
	if(flags < 0){
		perror("fcntl0");
		exit(1);
	}

	printf("%d before flag = %lx\n", i, flags);

	flags &= ~O_NONBLOCK;

	ret = fcntl(fifo, F_SETFL, flags);
	if(ret < 0){
		perror("fcntl1");
		exit(1);
	}

	flags = fcntl(fifo, F_GETFL, 0);
	if(flags < 0){
		perror("fcntl0");
		exit(1);
	}

	printf("%d after flag = %lx\n", i, flags);

	char buf[1024];

	while(1){
	ret = read(fifo, buf, 1024);
	if(ret <= 0){
		perror("read ok");
		exit(1);
	}

	write(1, buf, ret);
	}

	close(fifo);

	return 0;
}
