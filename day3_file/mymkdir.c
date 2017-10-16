#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
	int ret;

	ret = mkdir(argv[1], 0775);
	if(ret < 0){
		perror("mkdir error");
		exit(1);
	}
	
	return 0;
}
