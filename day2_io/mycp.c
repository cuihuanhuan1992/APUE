#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int src_fd, dst_fd;
	char buf[BUFSIZ];
	int ret = 1;

	src_fd = open(argv[1], O_RDONLY);
	if(src_fd < 0){
		perror("src_fd");
		exit(1);
	}

	dst_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0660);
	if(dst_fd < 0){
		perror("dst_fd");
		exit(1);
	}

	while(ret){
		ret = read(src_fd, buf, BUFSIZ);
		if(ret < 0){
			perror("read src_fd");
			exit(1);
		}

		ret = write(dst_fd, buf, ret);
		if(ret < 0){
			perror("write dst_fd");
			exit(1);
		}
	}	

	close(dst_fd);
	close(src_fd);

	return 0;
}

