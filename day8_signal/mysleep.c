#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void do_signal(int no)
{

}

int mysleep(int sec)
{
	int ret;

	signal(SIGALRM, do_signal);
	ret = alarm(sec);
	//此处有时间窗，如果在这个时候来了信号，则进程有肯能永远睡眠
	pause();

	return ret;
}

int main(void)
{
	int ret;

	printf("i am here\n");
	ret = mysleep(5);
	printf("i am here %d\n", ret);

	return 0;
}
