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
		printf("before set:child pid=%d sid=%d\n", (int)getpid(), (int)getsid(0));
		//ret = setpgrp();
		//ret = setsid();
		//if(ret < 0){
		//	perror("child setsid");
		//	exit(1);
		//}
		printf("after set:child pid=%d sid=%d\n", (int)getpid(), (int)getsid(0));
		while(1);
		exit(0);
	}

	printf("beforce set:father pid=%d sid=%d\n", (int)getpid(), (int)getsid(0));

	//ret = setsid();
	//if(ret < 0){
	//	perror("father setsid");
	//	exit(1);
	//}

	//printf("after set:father pid=%d sid=%d\n", (int)getpid(), (int)getsid(0));

	//while(1);
	exit(0);

	return 0;
}
