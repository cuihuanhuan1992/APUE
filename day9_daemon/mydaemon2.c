#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/stat.h>

#define LOCK_FILE	"/var/run/mylockfile.pid"

int main(void)
{
	int fd;

	fd = open(LOCK_FILE, O_EXCL | O_CREAT, 0666);
	if(fd < 0){
		fprintf(stderr, "already runing\n");
		exit(1);
	}

	if(daemon(0, 0))
		return -1;	

	while(1){
		printf("jhafdlajlfdj\n");
	}

	return 0;
}







