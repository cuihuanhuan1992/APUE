#include <stdio.h>
#include <unistd.h>

int main(void)
{
	printf("phread id %u\n", pthread_self());
	printf("process id %d\n", (int)getpid());

	while(1);

	return 0;
}
