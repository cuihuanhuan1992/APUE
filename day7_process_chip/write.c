#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
	int ret;

	ret = write(1, "hello i am write", 16);
	if(ret < 0){
		perror("write error error");
		exit(1);
	}

	return 0;
}
