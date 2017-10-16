#include <stdio.h>

int main(void)
{
	char *s = "hello 100 a";
	char buf[64];
	int a;
	char c = 'a';

	sscanf(s, "%s %d %c",buf, &a, &c);

	printf("%s %d %c\n", buf, a, c);

	return 0;
}
