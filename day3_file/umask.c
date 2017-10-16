#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(void)
{
	int ret;

	umask(0);
	ret = creat("umask0.txt", 0777);
	if(ret < 0){
		perror("creat umask0.txterror");
		exit(1);
	}

	close(ret);

	umask(0333);
	
	ret = creat("umask1.txt", 0777);
	if(ret < 0){
		perror("creat umask1.txterror");
		exit(1);
	}

	close(ret);

	return 0;	
}
