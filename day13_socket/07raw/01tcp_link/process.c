#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	pid_t pid;
	int i;
	int count = atoi(argv[3]);

	for(i = 0; i < count; i++){
		pid = fork();
		if(pid < 0){
			perror("fork");
			exit(1);
		}else if(pid == 0){
			execl("/bin/tcp_link", "tcp_link", argv[1], argv[2], NULL);
			perror("execl");
		}
	}

	for(i = 0; i < count; i++)
		wait(NULL);

	return 0;
}
