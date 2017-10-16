#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int ret;

	ret = link(argv[1], argv[2]);
	if(ret < 0){
		perror("link error");
		exit(1);
	}

	return 0;
}
