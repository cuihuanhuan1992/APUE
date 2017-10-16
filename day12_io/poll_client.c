#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/fcntl.h>

int main(int argc, char *argv[])
{
	int ret;
	int fifo;

	fifo = open(argv[1], O_WRONLY);
	if(fifo < 0){
		perror("open fifo");
		exit(1);
	}

	while(1){
		ret = write(fifo, argv[1], strlen(argv[1]) + 1);
		if(ret < 0){
			perror("write error");
			exit(1);
		}

		sleep(2);
	}	

	close(fifo);

	return 0;
}


















