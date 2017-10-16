#include <stdio.h>

int main(void)
{
	FILE *tmp;

	tmp = freopen("./tmp.txt", "w", stdout);
	if(tmp == NULL){
		perror("freopen");
		exit(1);
	}

	printf("hello world\n");

	fclose(tmp);

	return 0;
}
