#include <stdio.h>
#include <linux/input.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/fcntl.h>

int main(void)
{
	//mouse /dev/input/mouse2
	//key	/dev/input/event1

	int key, mouse;
	struct input_event event;
	int ret;
	int maxfd;
	fd_set readset;
	int count;
	struct timeval t;
	
	key = open("/dev/input/event1", O_RDONLY);
	mouse = open("/dev/input/event4", O_RDONLY);
	if(key < 0 || mouse < 0){
		perror("open");
		exit(1);
	}

	maxfd = key>mouse?key:mouse;

	while(1){
		FD_ZERO(&readset);
		FD_SET(key, &readset);
		FD_SET(mouse, &readset);
		t.tv_sec = 5;	
		t.tv_usec = 0;
		count = select(maxfd + 1, &readset, NULL, NULL, &t);
		if(count < 0){
			perror("select");
			continue;
		}else if(count == 0){
			printf("no input 5s\n");
			continue;
		}

		while(count--){
			if(FD_ISSET(mouse, &readset)){
				ret = read(mouse, &event, sizeof(struct input_event));	
				if(ret < 0){
					perror("read mouse");
				}else{
					printf("mouse:type=%d\n", event.type);
					printf("mouse:code=%d\n", event.code);
					printf("mouse:value=%d\n", event.value);
				}
			}

			if(FD_ISSET(key, &readset)){
				ret = read(key, &event, sizeof(struct input_event));	
				if(ret < 0){
					perror("read key");
				}else{
					printf("key:type=%d\n", event.type);
					printf("key:code=%d\n", event.code);
					printf("key:value=%d\n", event.value);
				}
			}
		}
	}

	close(key);
	close(mouse);

	return 0;
}
