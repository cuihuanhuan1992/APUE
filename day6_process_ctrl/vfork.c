#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	pid_t  pid;
	int a = 5, b = 6;

	write(1, "nihao\n", 6);

	printf("befor fork\n");

	pid = vfork();
	if(pid < 0){
		perror("fork");
		exit(1);
	}else if(pid == 0){
		printf("I am child\n");
		a++;
		b++;
		//_exit(0);	
	}

	printf("%d   a = %d\tb = %d\n", pid, a, b);

	//exit(0);
	return 0;
}















































