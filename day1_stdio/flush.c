#include <stdio.h>

int main(void)
{
#if 0
	printf("nihao");

	fprintf(stderr, "hello");

	printf("\n");

	//输出结果：hellonihao
#elif 0

	setbuf(stdout, NULL);//关闭缓冲

	printf("nihao");

	fprintf(stderr, "hello");
	
	//输出结果：nihaohello
#elif 0
	char buf[BUFSIZ];

	setbuf(stdout, buf);//全缓冲	

	printf("nihao");

//	fflush(stdout);

	fprintf(stderr, "hello");

	printf("\n");
	
	sleep(2);

	//输出结果：hello 2s后 nihao
#elif 0	
	char c;

	char buf[BUFSIZ];

	setbuf(stdin, NULL);//行缓冲	

	c = getchar();

	printf("%c\n", c);	
#elif 1
	setvbuf(stdout, NULL, _IOLBF, 0);
	printf("hello");
	fprintf(stderr, "nihao");
	printf("\n");
	sleep(2);
#endif

	return 0;
}







