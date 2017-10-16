#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void handler(int no)
{
	printf("alarm\n");
	alarm(1);
}

int main(void)
{
	int ret;

	if(signal(SIGALRM, handler) == SIG_ERR){
                perror("signal");
                exit(1);
        }       

	ret = alarm(1);

	printf("ret = %d\n", ret);

//	pause();
	while(1);

	printf("i am here\n");

	return 0;
}
