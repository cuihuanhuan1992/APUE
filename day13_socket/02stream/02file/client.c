#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */

int main(int argc, char *argv[])
{
	int sfd;
	int ret;
	int len = 0;
	int fd;
	struct sockaddr_in heraddr;
	char buf[BUFSIZ];

	fd = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0666);
	if(fd < 0){
		perror("open");
		exit(1);
	}
	
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

printf("fdasfkjfkf\n");
	ret = write(sfd, argv[2], strlen(argv[2]) + 1);
	if(ret < 0){
		perror("write");
		exit(1);
	}

	while(1){
		ret = recv(sfd, buf, BUFSIZ, 0);
		if(ret < 0){
			perror("recv");
			exit(1);
		}
		if(ret == 0){
			break;
		}
		len += ret;
		ret = write(fd, buf, ret);
		if(ret < 0){
			perror("store");
			exit(1);
		}
	}

	close(fd);
	close(sfd);

	return 0;
}







