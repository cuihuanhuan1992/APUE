#include <stdio.h>
#include <setjmp.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

jmp_buf jmp;
int e = 10;

void nihao(void)
{
	/* error */
	longjmp(jmp, 1);
}

void world(void)
{
	nihao();
}

void hello(int n)
{
	int ret;
	int a = 5;
	static int b = 6;
	volatile int c = 7;
	register int d = 8;

      if((ret = setjmp(jmp))){
                printf("jmp :a = %d b = %d c = %d d = %d n = %d e = %d\n", a, b, c, d, n, e);
                exit(1);
        }


	if((ret = setjmp(jmp))){
		printf("jmp here:a = %d b = %d c = %d d = %d n = %d e = %d\n", a, b, c, d, n, e);
		exit(1);
	}

	a += 1;
	b += 1;
	c += 1;
	d += 1;
	n += 1;
	e += 1;

	world();
}

int main(void)
{
	int a = 9;

	hello(a);

	return 0;
}

