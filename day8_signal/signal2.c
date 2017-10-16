#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void handler(int no)
{
	volatile int i, j;
	volatile int a = 0;
	/*这里有时间窗口*/
	//signal(SIGUSR1, handler);	
	printf("no = %d\n", no);
	for(i = 0; i < 10000000; i++)
		for(j = 0; j < 50; j++)
		a++;
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

	while(1);

	return 0;
}
