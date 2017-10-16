#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/types.h>

int main(void)
{
	int ufd;
	int ret;
	struct sockaddr_un myaddr;
	int fd;
	char buf[1024];

	ufd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(ufd < 0){
		perror("ufd");
		exit(1);
	}

	myaddr.sun_family = AF_UNIX;
	strcpy(myaddr.sun_path, "/tmp/server.socket");

	ret = bind(ufd, (struct sockaddr *)&myaddr, sizeof(myaddr.sun_family) + strlen(myaddr.sun_path));
	if(ret < 0){
		perror("bind");
		exit(1);
	}

	ret = listen(ufd, 20);
	if(ret < 0){
		perror("listen");
		exit(1);
	}	
	
	while(1){
		fd = accept(ufd, NULL, NULL);
		ret = read(fd, buf, sizeof(buf));
		if(ret < 0){
			perror("read");
			exit(1);
		}
		buf[ret] = 0;
		printf("recv:%s\n", buf);
		write(fd, "nihao\n", 7);
		close(fd);
	}

	close(ufd);
}














