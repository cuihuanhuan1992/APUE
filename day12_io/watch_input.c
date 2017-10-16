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
	
	mouse = open("/dev/input/event4", O_RDONLY | O_NONBLOCK);
	key = open("/dev/input/event1", O_RDONLY | O_NONBLOCK);
	//key = open("/dev/input/mouse2", O_RDONLY);
	//mouse = open("/dev/input/event1", O_RDONLY);
	if(key < 0 || mouse < 0){
		perror("open");
		exit(1);
	}

	while(1){
		ret = read(mouse, &event, sizeof(struct input_event));	
		if(ret < 0){
			;//perror("read mouse");
		}else{
			printf("mouse:type=%d\n", event.type);
			printf("mouse:code=%d\n", event.code);
			printf("mouse:value=%d\n", event.value);
		}
			
		ret = read(key, &event, sizeof(struct input_event));	
		if(ret < 0){
			;//perror("read key");
		}else{
			printf("key:type=%d\n", event.type);
			printf("key:code=%d\n", event.code);
			printf("key:value=%d\n", event.value);
		}
	}

	close(key);
	close(mouse);

	return 0;
}
