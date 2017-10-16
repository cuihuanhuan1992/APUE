#include <stdio.h>
#include <setjmp.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>

int main(void)
{
	int ret;
	struct rlimit limit;

	getrlimit(RLIMIT_NOFILE, &limit);
	/*if error*/
	if(limit.rlim_cur != RLIM_INFINITY)
		printf("RLIMIT_NOFILE soft:%d\n", (int)limit.rlim_cur);
	if(limit.rlim_max != RLIM_INFINITY)
		printf("RLIMIT_NOFILE hard:%d\n", (int)limit.rlim_max);

	limit.rlim_cur = 3;

	ret = setrlimit(RLIMIT_NOFILE, &limit);
	if(ret < 0){
		perror("setrlimt");
		exit(1);
	}

	getrlimit(RLIMIT_NOFILE, &limit);
	/*if error*/
	if(limit.rlim_cur != RLIM_INFINITY)
		printf("RLIMIT_NOFILE soft:%d\n", (int)limit.rlim_cur);
	if(limit.rlim_max != RLIM_INFINITY)
		printf("RLIMIT_NOFILE hard:%d\n", (int)limit.rlim_max);

	if(open("limit.c", O_RDONLY) < 0){
		perror("open file");
		exit(1);
	}

	return 0;
}






