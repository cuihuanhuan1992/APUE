#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
	int ret;

	ret = atoi(argv[2]);

	ret %= 1000;
	
	ret = ret / 100 * 64 + (ret % 100 ) / 10 * 8 + ret % 10;

	umask(0777);//不会对chmod产生影响

	ret = chmod(argv[1], ret);
	if(ret < 0){
		perror("chmod error");
		exit(1);
	}
	
	return 0;
}
