#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(void)
{
	if(daemon(0, 0))
		return -1;	

	while(1){
		printf("jhafdlajlfdj\n");
	}

	return 0;
}







