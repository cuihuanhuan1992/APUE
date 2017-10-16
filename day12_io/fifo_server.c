#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/fcntl.h>

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

	fifo[0] = open(FIFO1, O_RDONLY | O_NONBLOCK);
	fifo[1] = open(FIFO2, O_RDONLY | O_NONBLOCK);
	fifo[2] = open(FIFO3, O_RDONLY | O_NONBLOCK);
	fifo[3] = open(FIFO4, O_RDONLY | O_NONBLOCK);
	if(fifo[1] < 0 || fifo[2] < 0 || fifo[3] < 0 || fifo[0] < 0){
		perror("open fifo");
		exit(1);
	}

	max=fifo[0]>fifo[1]?fifo[0]:fifo[1];	
	max=max>fifo[2]?max:fifo[2];	
	max=max>fifo[3]?max:fifo[3];	

	while(1){
		FD_ZERO(&readset);
		FD_SET(fifo[0], &readset);
		FD_SET(fifo[1], &readset);
		FD_SET(fifo[2], &readset);
		FD_SET(fifo[3], &readset);
		count = select(max + 1, &readset, NULL, NULL, NULL);
		if(count < 0){
			perror("select");
			continue;
		}

		for(i = 0; i < 4; i++){
			if(FD_ISSET(fifo[i], &readset)){
				ret = read(fifo[i], buf, 1024);
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


















