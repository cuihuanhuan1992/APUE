#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void handler(int no)
{
	printf("no = %d\n", no);
}

int main(void)
{
	if(signal(SIGINT, SIG_IGN) == SIG_ERR){
		perror("signal");
		exit(1);
	}	

	if(signal(SIGQUIT, handler) == SIG_ERR){
		perror("signal");
		exit(1);
	}	

	if(signal(SIGUSR1, handler) == SIG_ERR){
                perror("signal");
                exit(1);
        }       

	if(signal(SIGKILL, handler) == SIG_ERR){
                perror("signal sigkill");
  //              exit(1);
        }       

	sleep(12000);
	printf("i am over\n");
	//pause();
//	while(1);

	return 0;
}
