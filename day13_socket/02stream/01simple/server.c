#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */

int main(int argc, char *argv[])
{
	int sfd;
	int len = 0;
	int rfd;
	char buf[BUFSIZ];
	int ret;
	struct sockaddr_in myaddr, heraddr;
	socklen_t addr_len = sizeof(heraddr);

	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sfd < 0){
		perror("socket");
		exit(1);
	}

	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(1235);
	myaddr.sin_addr.s_addr = INADDR_ANY;	

	ret = bind(sfd, (struct sockaddr *)&myaddr, sizeof(myaddr));
	if(ret < 0){
		perror("bind");
		exit(1);
	}

	ret = listen(sfd, 20);
	if(ret < 0){
		perror("listen");
		exit(1);
	}

	//rfd = accept(sfd, NULL, NULL);
	rfd = accept(sfd, (struct sockaddr *)&heraddr, &addr_len);
	if(rfd < 0){
		perror("accept");	
		exit(1);
	}

	while(1){
		ret = read(rfd, buf + len, BUFSIZ);
		if(ret < 0){
			perror("read");
			exit(1);
		}
		len += ret;
		if(ret == 0){
			break;
		}
	}

	ret = write(1, buf, len);
	if(ret < 0){
		perror("write");
		exit(1);
	}

	printf("\n");

	close(rfd);
	close(sfd);

	return 0;
}







