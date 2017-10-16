#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int ret;

	ret = kill(atoi(argv[2]), atoi(argv[1]));
	if(ret < 0){
		perror("kill");
		exit(1);
	}

	return 0;
}
