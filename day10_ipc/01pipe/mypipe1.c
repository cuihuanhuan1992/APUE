#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd[2];
	char buf[32];

	if(pipe(fd) < 0){
		perror("pipe");
		exit(1);
	}

	write(fd[1], "hello world", 11);
	/*if error*/

	read(fd[0], buf, 11);
	/*if error*/

	write(1, buf, 11);
	/*if error*/

	close(fd[0]);
	close(fd[1]);
	
	return 0;
}
