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
	struct sockaddr_un heraddr;
	int fd;
	char buf[1024];

	ufd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(ufd < 0){
		perror("ufd");
		exit(1);
	}

	heraddr.sun_family = AF_UNIX;
	strcpy(heraddr.sun_path, "/tmp/server.socket");

	//ret = bind(ufd, (struct sockaddr *)&myaddr, sizeof(myaddr.sun_family) + strlen(myaddr.sun_path));
	//if(ret < 0){
	//	perror("bind");
	//	exit(1);
	//}

	//ret = listen(ufd, 20);
	//if(ret < 0){
	//	perror("listen");
	//	exit(1);
	//}	
	
	ret = connect(ufd, (struct sockaddr *)&heraddr, sizeof(heraddr.sun_family) + strlen(heraddr.sun_path));
	if(ret < 0){
		perror("connect");
		exit(1);
	}

	ret = write(ufd, "hello\n", 7);
	if(ret < 0){
		perror("write");
		exit(1);
	}

	ret = read(ufd, buf, sizeof(buf));
	if(ret < 0){
		perror("read");
		exit(1);
	}
	buf[ret] = 0;
	printf("recv:%s\n", buf);

	close(ufd);
}














