#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	pid_t  pid;
	int a = 5, b = 6;

	write(1, "nihao\n", 6);

	printf("befor fork\n");

	pid = fork();
	if(pid < 0){
		perror("fork");
		exit(1);
	}else if(pid == 0){
		printf("I am child\n");
		a++;
		b++;
		//exit(0);
		_exit(0);
	}

	printf("%d   a = %d\tb = %d\n", pid, a, b);

	return 0;
}















































