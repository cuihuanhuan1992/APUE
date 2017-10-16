#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	char buf[PATH_MAX];

	printf("pwd:%s\n", getcwd(buf, PATH_MAX));

	return 0;
}
