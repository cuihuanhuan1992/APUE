#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

void do_signal(int no)
{
	printf("signal here\n");
}

int main(int argc, char *argv[])
{
	signal(SIGINT, do_signal);

	sleep(5);

	raise(2);

	printf("signal over\n");

	return 0;
}
