#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>

int main(void)
{
	char buf[1024];
	int ret;
	long flags;

	flags = fcntl(0, F_GETFL, 0);
	if(flags < 0){
		perror("fcntl0");
		exit(1);
	}

	ret = fcntl(0, F_SETFL, flags | O_NONBLOCK);
	if(ret < 0){
		perror("fcntl1");
		exit(1);
	}

	ret = read(0, buf, 1024);
	if(ret < 0){
		perror("read");
		exit(1);
	}

	ret = write(1, buf, ret);
	if(ret < 0){
		perror("write");
		exit(1);
	}

	return 0;
}
