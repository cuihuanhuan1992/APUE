#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <pwd.h>
#include <dirent.h>

int main(int argc, char *argv[])
{
	int ret;
	DIR *dir;
	struct dirent *itemp;
	struct stat sbuf;
	int pos;

	char buf[BUFSIZ] = {0};

	dir = opendir(argv[1]);
	if(!dir){
		perror("opendir");
		exit(1);
	}

	strcat(buf, argv[1]);
	strcat(buf, "/");
	pos = strlen(buf);	
	
	while((itemp = readdir(dir)) != NULL){
		buf[pos] = 0;
		strcat(buf, itemp->d_name);
		ret = stat(buf, &sbuf);
		if(ret < 0){
			perror("stat");
			exit(1);
		}
		printf("%s\t%lu\t%s\t%s\n", itemp->d_name, itemp->d_ino, getpwuid(sbuf.st_uid)->pw_name, getpwuid(sbuf.st_gid)->pw_name);
	}

	closedir(dir);
	
	return 0;
}
