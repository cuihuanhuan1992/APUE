#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>

int main(void)
{
	int ret;
	int ufd;
	char buf[1024];
	struct sockaddr_un myaddr;
	struct sockaddr_un heraddr;
	socklen_t len = sizeof(heraddr);
	pid_t pid;
	int i = 0;

	ufd = socket(AF_UNIX, SOCK_DGRAM, 0);	
	if(ufd < 0){
		perror("socket");
		exit(1);
	}

	myaddr.sun_family = AF_UNIX;
	strcpy(myaddr.sun_path, "/tmp/abc.socket");

	ret = bind(ufd, (struct sockaddr *)&myaddr, sizeof(myaddr.sun_family) + strlen(myaddr.sun_path));
	if(ret < 0){
		perror("bind");
		exit(1);
	}
	
	while(1){
		ret = recvfrom(ufd, buf, sizeof(buf), 0, (struct sockaddr *)&heraddr, &len);
		if(ret < 0){
			perror("recvfrom");
			exit(1);
		}
		buf[ret] = 0;
		printf("%s\n", buf);
		ret = sendto(ufd, "nihao\n", 7, 0, (struct sockaddr *)&heraddr, len);
		if(ret < 0){
			perror("sendto");
			exit(1);
		}
	}

	close(ufd);

	return 0;
}




