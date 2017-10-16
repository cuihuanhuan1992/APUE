#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/fcntl.h>

#include <arpa/inet.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */

#include <sys/ioctl.h>
#include <net/if.h>

int main(void)
{
	int sfd;
	int ret;
	char dst[64];
	int enable = 1;

	struct ifreq ifr;
	struct sockaddr_in *broadcast_addr;

	socklen_t len = sizeof(dst);

	sfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sfd < 0){
		perror("socket");
		exit(1);
	}

	strcpy(ifr.ifr_name, "wlan0");

	ret = ioctl(sfd, SIOCGIFBRDADDR, &ifr);
	if(ret < 0){
		perror("ioctl");
		exit(1);
	}

	broadcast_addr = (void *)&ifr.ifr_broadaddr;

	printf("%s\n", inet_ntop(AF_INET, &broadcast_addr->sin_addr, dst, len));

	ret = setsockopt(sfd, SOL_SOCKET, SO_BROADCAST, &enable, sizeof(enable));
	if(ret < 0){
		perror("setsockopt");
		exit(1);
	}

	broadcast_addr->sin_port = htons(12345);
	broadcast_addr->sin_family = AF_INET;

	while(1){
		ret = sendto(sfd, "nihao!", 7, 0, (const struct sockaddr *)broadcast_addr, sizeof(struct sockaddr));
		if(ret < 0){
			perror("send");
			exit(1);
		}
		sleep(1);
	}

	close(sfd);

	return 0;
}









