#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(void)
{
	char *env;
	char name[] = "NIHAO=nihao";

	env = getenv("HOME");

	printf("%s\n", env);

	env = getenv("PWD");

	printf("%s\n", env);

	//if(putenv(name) < 0){
	if(setenv("NIHAO", name, 1) < 0){
		perror("putenv");
		exit(1);
	}	
	
	name[7] = 'a';

	//unsetenv("NIHAO");

	env = getenv("NIHAO");

	printf("%s\n", env);
	
	return 0;
}

