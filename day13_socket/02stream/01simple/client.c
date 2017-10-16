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
	int ret;
	struct sockaddr_in heraddr;

	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sfd < 0){
		perror("socket");
		exit(1);
	}

	heraddr.sin_family = AF_INET;
	heraddr.sin_port = htons(1235);
	heraddr.sin_addr.s_addr = inet_addr(argv[1]);	

	ret = connect(sfd, (const struct sockaddr *)&heraddr, sizeof(heraddr));
	if(ret < 0){
		perror("connect");
		exit(1);
	}

	//ret = write(sfd, "nihao", 6);
	ret = send(sfd, "nihao", 6, 0);
	if(ret < 0){
		perror("write");
		exit(1);
	}

	//ret = write(sfd, "hello", 6);
	ret = sendto(sfd, "hello", 6, 0, NULL, 0);
	if(ret < 0){
		perror("write");
		exit(1);
	}

	ret = write(sfd, "world", 6);
	if(ret < 0){
		perror("write");
		exit(1);
	}

	close(sfd);

	return 0;
}







