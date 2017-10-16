#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>

void my_system(char *cmd)
{
	pid_t pid;

	pid = fork();
	if(pid < 0){
		perror("fork");
		exit(1);
	}else if(pid == 0){
		execlp("/bin/sh", "sh", "-c", cmd, NULL);
		perror("execlp");
		exit(1);
	}
	
	wait(NULL);
}

int main(int argc, char *argv[])
{
	//my_system("echo helloworld");
	//my_system("pwd");
	//my_system("ls");

	system("echo helloworld");
	system("pwd");
	system("cd /");
	system("ls");

	return 0;
}
