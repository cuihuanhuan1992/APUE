#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>          /* See NOTES */
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */

int main(int argc, char *argv[])
{
	int sfd;
	int ret;
	struct sockaddr_in myaddr, heraddr;
	socklen_t addr_len = sizeof(struct sockaddr);
	char addr_buf[64];
	int count = 100;
	char buf[3024];

	sfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sfd < 0){
		perror("socket");
		exit(1);
	}

	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(1234);
	myaddr.sin_addr.s_addr = INADDR_ANY;
	//myaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
//	myaddr.sin_addr.s_addr = inet_addr("192.168.1.201");

	ret = bind(sfd, (const struct sockaddr *)&myaddr, sizeof(myaddr));
	if(ret < 0){
		perror("bind");
		exit(1);
	}

	while(count--){
		ret = recvfrom(sfd, buf, sizeof(buf), 0, (void *)&heraddr, &addr_len);	
		printf("ret = %d\n", ret);
		if(ret < 0){
			perror("sendto");
			exit(1);
		}
		write(1, buf, ret);
		printf("from %s port %x\n", inet_ntop(AF_INET, &heraddr.sin_addr, addr_buf, sizeof(addr_buf)), ntohs(heraddr.sin_port));
	}

	close(sfd);

	return 0;
}




