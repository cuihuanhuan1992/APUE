#include <stdio.h>

int main(void)
{
	char buf[64];

	fgets(buf, 64, stdin);

	fputs(buf, stdout);

	return 0;
}
