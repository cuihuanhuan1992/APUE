#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void do_signal(int no)
{
	printf("SIGINT signal comming\n");
}

int main(void)
{
	sigset_t sigset, old_sigset;
	
	sigemptyset(&sigset);
	sigaddset(&sigset, SIGINT);
	sigprocmask(SIG_BLOCK, &sigset, &old_sigset);

	sigpending(&sigset);
	if(sigismember(&sigset, SIGINT))
		printf("sig unblock but occur SIGINT");

	printf("block\n");

	signal(SIGINT, do_signal);

	sleep(10);

	sigpending(&sigset);
	if(sigismember(&sigset, SIGINT))
		printf("sig unblock but occur SIGINT");

	sigprocmask(SIG_SETMASK, &old_sigset, NULL);

	printf("unblock\n");

	sleep(10);

	printf("will exit\n");

	return 0;
}
