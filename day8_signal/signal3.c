#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

static int c;

int add(int a, int b)
{
	c = a + b;

	sleep(10);

	return c;
}

void handler(int no)
{
	c = 100;
}

int main(void)
{
	if(signal(SIGUSR1, handler) == SIG_ERR){
                perror("signal");
                exit(1);
        }       

	if(signal(SIGUSR2, handler) == SIG_ERR){
                perror("signal");
                exit(1);
        }       

	printf("%d\n", add(10, 20));

	return 0;
}
