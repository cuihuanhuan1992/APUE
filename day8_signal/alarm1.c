#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <setjmp.h>

static jmp_buf env;

void do_signal(int no)
{
	longjmp(env, 1);
}

int main(void)
{
	int ret;
	char buf[32];

	signal(SIGALRM, do_signal);

	if(setjmp(env)){
		fprintf(stderr, "time is over\n");
		exit(1);
	}

	alarm(10);

	//在linux下信号打断read后会重启
	ret = read(0, buf, sizeof(buf));
	if(ret < 0){
		perror("read");
		exit(1);
	}

	alarm(0);

	ret = write(1, buf, ret);
	if(ret < 0){
		perror("write");
		exit(1);
	}

	return 0;
}
