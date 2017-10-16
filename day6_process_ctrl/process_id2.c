#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

void show_id(void)
{
	printf("pid:%d\n", (int)getpid());
	printf("ppid:%d\n", (int)getppid());

	printf("uid user:%s\n", getpwuid(getuid())->pw_name);
	printf("gid group:%s\n", getpwuid(getgid())->pw_name);

	printf("euid user:%s\n", getpwuid(geteuid())->pw_name);
	printf("egid group:%s\n", getpwuid(getegid())->pw_name);
}

int main(void)
{
	show_id();

	if(seteuid(1002) < 0)
		perror("setuid");; //test

	show_id();

	return 0;
}

