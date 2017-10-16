#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	FILE *file;
	char *s = "hello world";
	size_t ret;
	fpos_t pos;
	int success;
	char c;
	
	file = fopen("fops.txt", "w+");
	if(!file){
		fprintf(stderr, "fopen error");
		exit(1);
	}

	ret = fwrite(s, (size_t)(strlen(s) + 1), 1, file);
	if(ret != 1){
		fprintf(stderr, "fwrite error");
		exit(1);
	}

	//fseek(file, 0, SEEK_END);
	//fseek(file, 5, SEEK_SET);
	//fseek(file, 0, SEEK_SET);
	/* if error */

	//fseeko(file, 0, SEEK_SET);
	//fseeko(file, 0, SEEK_END);
	

	//printf("current pos:%ld\n", ftell(file));

	//printf("current pos:%ld\n", ftello(file));

	pos.__pos = 10;
	
	fsetpos(file, &pos);

	fread(&c, 1, 1, file);

	putchar(c);

	fgetpos(file, &pos);
	/* if error */

	printf("current pos:%ld\n", pos.__pos); 

	fclose(file);

	return 0;
}
