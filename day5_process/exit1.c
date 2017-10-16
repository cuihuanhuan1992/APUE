#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void fun1(void)
{
	printf("%s\n", __FUNCTION__);
}

void fun2(void)
{
	printf("%s\n", __FUNCTION__);
}

void fun3(void)
{
	printf("%s\n", __FUNCTION__);
}

int main(void)
{
	printf("hello world\n");

	atexit(fun1);
	atexit(fun2);
	atexit(fun3);
	atexit(fun2);

//	return 0;
//	_exit(0);
}

