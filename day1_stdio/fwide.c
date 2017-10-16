#include <stdio.h>
#include <wchar.h>

int main(void)
{
	int stream;

	stream = fwide(stdout, 0);

//	printf("stream = %d\n", stream);

//	putchar('a');

	//stream = fwide(stdin, -5);
	//
	//printf("stream = %d\n", stream);

	stream = fwide(stdout, 6);
	
//	printf("stream = %d\n", stream);

	putchar('a');
fflush(NULL);
	putchar('\n');
fflush(NULL);

	return 0;
}

