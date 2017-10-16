#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>

int main(int argc, char *argv[])
{
	int ret;
	DIR *dir;
	struct dirent *itemp, item;

	ret = chdir(argv[1]);
	if(ret < 0){
		perror("chdir");
		exit(1);
	}

	ret = open(argv[2], O_RDWR | O_CREAT, 0755);
	if(ret < 0){
		perror("open error");
		exit(1);
	}
	
	close(ret);
	
	return 0;
}
