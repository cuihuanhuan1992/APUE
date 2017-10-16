#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string.h>

#include <sys/socket.h>
#include <netpacket/packet.h>
#include <net/ethernet.h> /* the L2 protocols */

#include <net/if_arp.h>
#include <netinet/if_ether.h>

#define BUFSIZE	2048

int main(int argc, char *argv[])
{
	int packet_socket;
	char send_buf[BUFSIZE];
	char recv_buf[BUFSIZE];
	int ret;

	struct sockaddr_ll ll;

	struct ethhdr *eth = (void *)send_buf;
	struct ether_arp *arp = (void *)(eth + 1);

	char dst_mac[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
	char src_mac[6] = {0x00, 0x22, 0xfa, 0xee, 0x56, 0x3a};

	unsigned int dst_ip = inet_addr(argv[1]);
	unsigned int src_ip = inet_addr("192.168.1.201");

	ll.sll_family = AF_PACKET;
	ll.sll_ifindex = 3;

	packet_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));	
	if(packet_socket < 0){
		perror("socket");
		exit(1);
	}

	memcpy(eth->h_dest, dst_mac, 6);
	memcpy(eth->h_source, src_mac, 6);

	eth->h_proto = htons(ETH_P_ARP);

	arp->ea_hdr.ar_hrd = htons(ARPHRD_ETHER);
	arp->ea_hdr.ar_pro = htons(ETHERTYPE_IP);	
	arp->ea_hdr.ar_hln = 6;
	arp->ea_hdr.ar_pln = 4;
	arp->ea_hdr.ar_op = htons(ARPOP_REQUEST);

 	memcpy(arp->arp_sha, src_mac, 6);
 	memcpy(arp->arp_spa, &src_ip, 4);
 	memcpy(arp->arp_tha, dst_mac, 6);
 	memcpy(arp->arp_tpa, &dst_ip, 4);

	while(1){
		ret = sendto(packet_socket, send_buf, sizeof(struct ethhdr) + sizeof(struct ether_arp), 0, (struct sockaddr *)&ll, sizeof(ll));
		if(ret < 0){
			perror("sendto");
			exit(1);
		}

		again:
		ret = recvfrom(packet_socket, recv_buf,  sizeof(recv_buf), 0, NULL, NULL);
		if(ret < 0){
			perror("recvfrom");
			exit(1);
		}
		
		eth = (void *)recv_buf;
		arp = (void *)(eth + 1);

		if(!memcmp(eth->h_dest, src_mac, 6) && htons(eth->h_proto) == ETH_P_ARP && htons(arp->ea_hdr.ar_op) == ARPOP_REPLY)
			printf("her mac is :%02X:%02X:%02X:%02X:%02X:%02X\n", arp->arp_sha[0], arp->arp_sha[1],arp->arp_sha[2],arp->arp_sha[3],arp->arp_sha[4],arp->arp_sha[5]);
		else 
			goto again;
			

		sleep(1);
	}	

	return 0;
}






