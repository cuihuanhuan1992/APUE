#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int main(void)
{
	int ret;

	ret = (int)sysconf(_SC_THREAD_THREADS_MAX);
	if(ret < 0)
		perror("sysconf");

	printf("%d\n", ret);

	return 0;
}







