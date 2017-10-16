#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	char buf[L_tmpnam];
	char *s;
	FILE *file;

	s = tmpnam(buf);
	if(!s){
		fprintf(stderr, "tmpnam error\n");	
		exit(1);
	}

	file = fopen(s, "w+");
	if(!file){
		fprintf(stderr, "fopen error\n");
		exit(1);
	}

	fwrite("nihao-helloworld\n", 18, 1, file);

	fclose(file);

	return 0;
}



