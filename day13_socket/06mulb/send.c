#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/fcntl.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */

#define GROUP_PORT	12345
#define GROUP_IP	"224.5.2.1"

int main(void)
{
	int sfd;
	int ret;
	struct sockaddr_in broad_addr;
	char *buf = NULL;
	size_t len;

	sfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sfd < 0){
		perror("socket");
		exit(1);
	}
	
	broad_addr.sin_family = AF_INET;
	broad_addr.sin_port = htons(GROUP_PORT);
	broad_addr.sin_addr.s_addr = inet_addr(GROUP_IP);
	
	while(1){
		ret = getline(&buf, &len, stdin);	
		if(ret < 0){
			perror("getline");
			exit(1);
		}
		ret = sendto(sfd, buf, ret + 1, 0, (struct sockaddr *)&broad_addr, sizeof(struct sockaddr));
		if(ret < 0){
			perror("sendto");
			exit(1);
		}
	}

	free(buf);

	close(sfd);
	
	return 0;
}





