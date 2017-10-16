#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */

int main(int argc, char *argv[])
{
	int sfd;
	int ret;
	struct sockaddr_in heraddr, myaddr;
	socklen_t myaddr_len = sizeof(myaddr);
	int count = 100;
	char buf[4024];

	sfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sfd < 0){
		perror("socket");
		exit(1);
	}

	heraddr.sin_family = AF_INET;
	heraddr.sin_port = htons(1234);
	//heraddr.sin_addr.s_addr = inet_addr(argv[1]);
	inet_pton(AF_INET, argv[1], &heraddr.sin_addr);

	while(count--){
		ret = sprintf(buf, "hello socket %d\n", count);
		ret = sendto(sfd, buf, ret, 0, (const struct sockaddr *)&heraddr, sizeof(heraddr));	
		if(ret < 0){
			perror("sendto");
			exit(1);
		}
		getsockname(sfd, (struct sockaddr *)&myaddr, &myaddr_len);
		printf("ip:%s\nport:%x\n", inet_ntop(AF_INET, &myaddr.sin_addr, buf, sizeof(buf)), ntohs(myaddr.sin_port));
		printf("send %d\n", count);
		sleep(2);
	}

	close(sfd);

	return 0;
}




