#include <stdio.h>
#include <poll.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/fcntl.h>

#define ARRAY_SIZE(p) (sizeof(p) / sizeof(typeof(p[0])))

#define FIFO1	"/tmp/myfifo1" 
#define FIFO2	"/tmp/myfifo2"
#define FIFO3	"/tmp/myfifo3"
#define FIFO4	"/tmp/myfifo4"

int main(void)
{
	int fifo[4];
	int max;
	fd_set readset;
	int count;
	char buf[1024];
	int ret , i;
	struct pollfd fds[4];

	fifo[0] = open(FIFO1, O_RDONLY | O_NONBLOCK);
	fifo[1] = open(FIFO2, O_RDONLY | O_NONBLOCK);
	fifo[2] = open(FIFO3, O_RDONLY | O_NONBLOCK);
	fifo[3] = open(FIFO4, O_RDONLY | O_NONBLOCK);
	if(fifo[1] < 0 || fifo[2] < 0 || fifo[3] < 0 || fifo[0] < 0){
		perror("open fifo");
		exit(1);
	}

	for(i = 0; i < ARRAY_SIZE(fds); i++){
		fds[i].fd = fifo[i];
		fds[i].events |= POLLIN;
	}

	while(1){
		count = poll(fds, ARRAY_SIZE(fds), -1);
		if(count < 0){
			perror("poll");
			continue;
		}

		for(i = 0; i < ARRAY_SIZE(fds); i++){
			if(fds[i].revents & POLLIN){
				ret = read(fds[i].fd, buf, 1024);
				if(ret < 0){
					fprintf(stderr, "read fifo %d error\n", i);
				}else{
					printf("%d:%s\n", i, buf);	
				}
			}
		}
	}	

	for(i = 0; i < 4; i++) close(fifo[i]);

	return 0;
}


















