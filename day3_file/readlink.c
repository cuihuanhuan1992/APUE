#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int ret;
	char buf[1024];

	ret = readlink(argv[1], buf, sizeof(buf));
	if(ret < 0){
		perror("rename error");
		exit(1);
	}

	buf[ret] = 0;

	printf("buf:%s\n", buf);

	return 0;
}
