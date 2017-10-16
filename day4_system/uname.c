#include <utmp.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
	struct utsname un;
	int ret;
	char hn[1024];

	ret = uname(&un);
	if(ret < 0){
		perror("uname");
		exit(1);
	}
	
	printf("sysname:%s\n", un.sysname);
        printf("nodename:%s\n", un.nodename);
        printf("release:%s\n", un.release);
        printf("version:%s\n", un.version);
        printf("machine:%s\n", un.machine);

	ret = gethostname(hn, 1024);
	if(ret < 0){
		perror("gethostname");
		exit(1);
	}

	printf("host name:%s\n", hn);

	return 0;
}
