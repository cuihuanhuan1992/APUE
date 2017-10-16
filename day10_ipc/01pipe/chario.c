#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	char *v = NULL;
	size_t n;
	int ret;
	int i;

	if((ret = getline(&v, &n, stdin)) < 0){
		perror("getline");
		exit(1);
	}

	v[ret - 1] = 0;

	for(i = 0; i < ret - 1; i++){
		if(v[i] <= 'z' && v[i] >= 'a')	
			v[i] = v[i] - 32;
	}	

	printf("%s", v);	
	fflush(stdout);

	free(v);

	return 0;
}
