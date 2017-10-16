#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

void process_state(int no)
{
	printf("father\n");
}

int main(void)
{
	//signal(SIGINT, process_state);
	signal(SIGINT, SIG_IGN);
	/*if error*/

	sleep(100);

	execl("./while", "while", NULL);

	perror("execl");

	return 0;
}












