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
	struct sockaddr_in myaddr, heraddr;
	struct ip_mreqn broadcast;
	char buf[2048];
	int count = 0;
	socklen_t len = sizeof(heraddr);

	sfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sfd < 0){
		perror("socket");
		exit(1);
	}
	
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(GROUP_PORT);
	myaddr.sin_addr.s_addr = inet_addr(GROUP_IP);

	ret = bind(sfd, (struct sockaddr *)&myaddr, sizeof(struct sockaddr));
	if(ret < 0){
		perror("bind");
	}

	broadcast.imr_multiaddr.s_addr = inet_addr(GROUP_IP);
	broadcast.imr_address.s_addr = INADDR_ANY;

	ret = setsockopt(sfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &broadcast, sizeof(broadcast));
	if(ret < 0){
		perror("setsockopt");
		exit(1);
	}

	while(1){
		ret = recvfrom(sfd, buf, sizeof(buf), 0, (struct sockaddr *)&heraddr, &len);
		if(ret < 0){
			perror("sendto");
			exit(1);
		}
		printf("recv:%s\n", buf);
		if(count++ == 10){
			ret = setsockopt(sfd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &broadcast, sizeof(broadcast));
			if(ret < 0){
				perror("setsockopt");
				exit(1);
			}
		}
	}

	free(buf);

	close(sfd);
	
	return 0;
}





