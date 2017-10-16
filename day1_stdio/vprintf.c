#include <stdio.h>
#include <stdarg.h>

int myprintf(const char *fmt, ...)
{
	va_list ap;
	int ret;

	va_start(ap, fmt);
	
	ret = vprintf(fmt, ap);

	va_end(ap);

	return  ret;
}

int main(void)
{
	myprintf("nihao %#x\n", 100);	
	return 0;
}
