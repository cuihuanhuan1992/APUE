#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
	int ret;

	ret = rmdir(argv[1]);
	if(ret < 0){
		perror("rmdir error");
		exit(1);
	}
	
	return 0;
}
