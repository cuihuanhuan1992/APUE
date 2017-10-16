#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/stat.h>

int daemonize(int nochdir, int noclose)
{
	pid_t pid;
	int i;
	int fd;
	struct rlimit rlim;

	umask(0);	
	
	if(!nochdir){
		if(chdir("/") < 0){
			return -1;
		}
	}

	//确保进程不是组长
	pid = fork();	
	if(pid < 0){
		return -1;
	}else if(pid){
		exit(0);
	}

	setsid();

	//确保进程不是会话首进程
	pid = fork();
	if(pid < 0){
		return -1;
	}else if(pid){
		exit(0);
	}

	//关闭该关闭的文件
	if(getrlimit(RLIMIT_NOFILE, &rlim) < 0){
		return -1;
	}

	for(i = 3; i < rlim.rlim_cur; i++)
		close(i);

	//重定向0，1，2
	if(!noclose){
		close(0);
		close(1);
		close(2);

		fd = open("/dev/null", O_RDWR);
		if(fd < 0){
			return -1;
		}

		dup2(fd, 1);
		dup2(fd, 2);
	}

	return 0;
}

int main(void)
{
	if(daemonize(0, 0))
		return -1;	

	while(1){
		printf("jhafdlajlfdj\n");
	}

	return 0;
}







