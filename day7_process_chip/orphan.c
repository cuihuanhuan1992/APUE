#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>

int main(int argc, char *argv[])
{
	pid_t pid;

	pid = fork();
	if(pid == 0)
		while(1)
			sleep(1);
	pid = fork();
	if(pid == 0){
		pid = fork();	
		if(pid == 0){
			while(1) sleep(1);
		}
		while(1)
			sleep(1);
	}

	pid = fork();
	if(pid == 0)
		while(1)
			sleep(1);

	return 0;
}
