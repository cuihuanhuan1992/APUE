#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
	int ret;
	char buf[1024];

	ret = read(0, buf, sizeof(buf));
	if(ret < 0){
		perror("read error");
		exit(1);
	}

	write(1, "i am read", 10);

	ret = write(1, buf, ret);
	if(ret < 0){
		perror("write error");
		exit(1);
	}

	return 0;
}
