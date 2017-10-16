#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	int a;
	FILE *file;
	char buf[64];
	char c;

	file = fopen("fscanf.txt", "w+");
	if(!file){
		fprintf(stderr, "fopen error");
		exit(1);
	}

	fprintf(file, "%s %d %c\n", "uplooking", 7, 'b');

	fseek(file, SEEK_SET, 0);

	fscanf(file, "%s %d %c", buf, &a, &c);

	//fscanf(stdin, "%d", &a);
	//printf("a = %d\n", a);

	printf("%s  %d   %c\n", buf, a, c);	

	fclose(file);

	return 0;
}
