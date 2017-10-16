#include <stdio.h>

int main(void)
{
	char buf[10];
	int ret;

	ret = snprintf(buf, 10, "nihao");

	printf("ret = %d\n", ret);
	printf("%s\n", buf);

	return 0;
}
