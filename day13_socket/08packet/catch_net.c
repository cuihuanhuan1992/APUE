#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netpacket/packet.h>
#include <net/ethernet.h> /* the L2 protocols */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> 

void unpack(char *buf)
{
	struct ethhdr *eth;
	struct iphdr *ip;
	struct in_addr ins, ind;


	eth = (void *)buf;

	printf("源MAC：%02X:%02X:%02X:%02X:%02X:%02X---->", \
			eth->h_source[0], \
			eth->h_source[1], \
			eth->h_source[2], \
			eth->h_source[3], \
			eth->h_source[4], \
			eth->h_source[5]);

	printf("目标MAC：%02X:%02X:%02X:%02X:%02X:%02X\t", \
			eth->h_dest[0], \
			eth->h_dest[1], \
			eth->h_dest[2], \
			eth->h_dest[3], \
			eth->h_dest[4], \
			eth->h_dest[5]);

	switch(htons(eth->h_proto)){
		case ETH_P_IP:
			ip = (void *)(eth + 1);
			ins.s_addr = ip->saddr;
			ind.s_addr = ip->daddr;
			printf("源IP：%s--->目的IP：", inet_ntoa(ins));	
			printf("%s\n", inet_ntoa(ind));	
			break;
		case ETH_P_ARP:
			printf("arp");
			break;
		case ETH_P_RARP:
			printf("rarp");
			break;
		default:
			break;
	}

	printf("\n");
}

int main(int argc, char *argv[])
{
	int packet_socket;
	int ret;
	char buf[2048];

	packet_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));	
	if(packet_socket < 0){
		perror("socket");	
		exit(1);
	}

	while(1){
		ret = recvfrom(packet_socket, buf, sizeof(buf), 0, NULL, NULL);
		if(ret < 0){
			perror("recvfrom");
			continue;
		}

		unpack(buf);	
	}

	close(packet_socket);

	return 0;
}
