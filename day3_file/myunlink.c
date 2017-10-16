#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int ret;

	ret = unlink(argv[1]);
	if(ret < 0){
		perror("link error");
		exit(1);
	}

	return 0;
}
