#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

int main(void)
{
	printf("pid:%d\n", (int)getpid());
	printf("ppid:%d\n", (int)getppid());

	printf("uid user:%s\n", getpwuid(getuid())->pw_name);
	printf("gid group:%s\n", getpwuid(getgid())->pw_name);

	printf("euid user:%s\n", getpwuid(geteuid())->pw_name);
	printf("egid group:%s\n", getpwuid(getegid())->pw_name);

	sleep(100);

	return 0;
}

