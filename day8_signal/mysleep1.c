#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

static jmp_buf buf;

void do_signal(int no)
{
	longjmp(buf, 1);
}

int mysleep(int sec)
{
	int ret;

	signal(SIGALRM, do_signal);
	if(setjmp(buf) != 0)	
		return 0;	
	ret = alarm(sec);//如果睡眠时间短还没来的及执行pause就产生了信号也没事了
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
