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
	struct sockaddr_un heraddr;
	struct sockaddr_un myaddr;
	pid_t pid;
	int i = 0;

	ufd = socket(AF_UNIX, SOCK_DGRAM, 0);	
	if(ufd < 0){
		perror("socket");
		exit(1);
	}

	myaddr.sun_family = AF_UNIX;
	strcpy(myaddr.sun_path, "/tmp/bcd.socket");

	ret = bind(ufd, (struct sockaddr *)&myaddr, sizeof(myaddr.sun_family) + strlen(myaddr.sun_path));
	if(ret < 0){
		perror("bind");
		exit(1);
	}

	heraddr.sun_family = AF_UNIX;
	strcpy(heraddr.sun_path, "/tmp/abc.socket");

	while(1){
		ret = sprintf(buf, "unix socket %d\n", i++);
		ret = sendto(ufd, buf, ret, 0, (struct sockaddr *)&heraddr, sizeof(heraddr.sun_family) + strlen(heraddr.sun_path));
		if(ret < 0){
			perror("sendot");
			exit(1);
		}
		ret = recvfrom(ufd, buf, sizeof(buf), 0, NULL, NULL);
		if(ret < 0){
			perror("recvfrom");
			exit(1);
		}

		buf[ret] = 0;
		printf("%s\n", buf);
	
		sleep(1);
	}

	close(ufd);

	return 0;
}
