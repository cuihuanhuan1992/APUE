#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
	int i;
	int ret;
	int num;
	int fifo;
	
	fifo = open(argv[1], O_WRONLY);
	if(fifo < 0){
		perror("open fifo");
		exit(1);
	}

	while(1){
		sleep(10);
		ret = write(fifo, argv[1], strlen(argv[1]) + 1); 
		if(ret < 0){
			perror("write fifo");
			continue;
		}
	}

	close(fifo);


	return 0;
}
