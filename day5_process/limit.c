#include <stdio.h>
#include <setjmp.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>

int main(void)
{
	int ret;
	struct rlimit limit;

	getrlimit(RLIMIT_AS, &limit);
	/*if error*/
	if(limit.rlim_cur != RLIM_INFINITY)
		printf("RLIMIT_AS soft:%d\n", (int)limit.rlim_cur);
	if(limit.rlim_max != RLIM_INFINITY)
		printf("RLIMIT_AS hard:%d\n", (int)limit.rlim_max);

	getrlimit(RLIMIT_CPU, &limit);
	/*if error*/
	if(limit.rlim_cur != RLIM_INFINITY)
		printf("RLIMIT_CPU soft:%d\n", (int)limit.rlim_cur);
	if(limit.rlim_max != RLIM_INFINITY)
		printf("RLIMIT_CPU hard:%d\n", (int)limit.rlim_max);

	getrlimit(RLIMIT_DATA, &limit);
	/*if error*/
	if(limit.rlim_cur != RLIM_INFINITY)
		printf("RLIMIT_DATA soft:%d\n", (int)limit.rlim_cur);
	if(limit.rlim_max != RLIM_INFINITY)
		printf("RLIMIT_DATA hard:%d\n", (int)limit.rlim_max);

	getrlimit(RLIMIT_FSIZE, &limit);
	/*if error*/
	if(limit.rlim_cur != RLIM_INFINITY)
		printf("RLIMIT_FSIZE soft:%d\n", (int)limit.rlim_cur);
	if(limit.rlim_max != RLIM_INFINITY)
		printf("RLIMIT_FSIZE hard:%d\n", (int)limit.rlim_max);

	getrlimit(RLIMIT_NOFILE, &limit);
	/*if error*/
	if(limit.rlim_cur != RLIM_INFINITY)
		printf("RLIMIT_NOFILE soft:%d\n", (int)limit.rlim_cur);
	if(limit.rlim_max != RLIM_INFINITY)
		printf("RLIMIT_NOFILE hard:%d\n", (int)limit.rlim_max);

	getrlimit(RLIMIT_NPROC, &limit);
	/*if error*/
	if(limit.rlim_cur != RLIM_INFINITY)
		printf("RLIMIT_NPROC soft:%d\n", (int)limit.rlim_cur);
	if(limit.rlim_max != RLIM_INFINITY)
		printf("RLIMIT_NPROC hard:%d\n", (int)limit.rlim_max);

	return 0;
}






