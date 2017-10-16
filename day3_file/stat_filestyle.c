#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	struct stat buf;
	int ret;

	ret = stat(argv[1], &buf);
	if(ret < 0){
		perror("stat");
		exit(1);
	}

	switch(buf.st_mode & S_IFMT){
		case S_IFSOCK:   
			printf("socket\n");
			break;
		case S_IFLNK:    
			printf("symbol link\n");
			break;
		case S_IFREG:    
			printf("reguler\n");
			break;
		case S_IFBLK:    
			printf("block\n");
			break;
		case S_IFDIR:    
			printf("dir\n");
			break;
		case S_IFCHR:    
			printf("char\n");
			break;
		case S_IFIFO:    
			printf("fifo\n");
			break;
		default:
			break;
	}

	return 0;
}






