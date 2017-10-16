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
	int fd;

	fd = open(argv[1], O_DIRECTORY);
	if(fd < 0){
		perror("open dir");
		exit(1);
	}

	//dir = opendir(argv[1]);
	dir = fdopendir(fd);
	if(!dir){
		perror("opendir");
		exit(1);
	}
	
	//while((itemp = readdir(dir)) != NULL){
	while(!readdir_r(dir, &item, &itemp)){
		if(!itemp)
			break;
		printf("%s\t%lu\n", itemp->d_name, itemp->d_ino);
	}

	closedir(dir);
	
	return 0;
}
