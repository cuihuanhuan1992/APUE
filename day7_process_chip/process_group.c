#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	pid_t pid;
	int ret;

	pid = fork();
	if(pid < 0){
		perror("fork");
		exit(1);
	}else if(pid == 0){
		//printf("child pid=%d pgid=%d\n", (int)getpid(), (int)getpgrp());
		printf("before set:child pid=%d pgid=%d\n", (int)getpid(), (int)getpgid(0));
		//ret = setpgrp();
		ret = setpgid(0, 0);
		if(ret < 0){
			perror("child setpgrp");
			exit(1);
		}
		printf("after set:child pid=%d pgid=%d\n", (int)getpid(), (int)getpgid(0));
		exit(0);
	}

	printf("beforce set:father pid=%d pgid=%d\n", (int)getpid(), (int)getpgrp());

	ret = setpgrp();
	if(ret < 0){
		perror("father setpgrp");
		exit(1);
	}

	printf("after set:father pid=%d pgid=%d\n", (int)getpid(), (int)getpgid(0));

	exit(0);

	return 0;
}
