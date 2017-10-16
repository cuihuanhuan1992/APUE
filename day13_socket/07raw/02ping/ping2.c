#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <time.h>

#include <sys/socket.h>
#include <netinet/in.h>

#include <arpa/inet.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */

#include <linux/icmp.h>

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

int icmp_pack(struct icmphdr *icmp, unsigned short no)
{
	int ret;

	icmp->type = 8;
	icmp->code = 0;
	icmp->checksum = 0;
	icmp->un.echo.id = htons(getpid());
	icmp->un.echo.sequence = htons(no);

	ret = clock_gettime(CLOCK_REALTIME, (void *)(icmp + 1));
	if(ret < 0){
		perror("clock_gettime");
		return -1;
	}

	icmp->checksum = check_sum((void *)icmp, sizeof(struct icmphdr) + sizeof(struct timespec));
	
	return 0;
}

int icmp_unpack(void *buf)
{
	char buffer[64];
	struct iphdr *ip = (void *)buf;
	struct icmphdr *icmp = (void *)(ip + 1);
	struct timespec *t = (void *)(icmp + 1);
	struct timespec t1;
	int ret;
	
	ret = clock_gettime(CLOCK_REALTIME, &t1);
        if(ret < 0){
                perror("clock_gettime");
                return -1;
        }

	if(icmp->type == 0 && icmp->code == 0 && icmp->un.echo.id == ntohs(getpid())){
		printf("%d bytes from %s icmp_seq = %d ttl = %d time = %lfms\n", ntohs(ip->tot_len), \
				inet_ntop(AF_INET, &ip->saddr, buffer, sizeof(buffer)), ntohs(icmp->un.echo.sequence), \
				ip->ttl, (t1.tv_sec - t->tv_sec) * 1000.0 + (t1.tv_nsec - t->tv_nsec) / 1000000.0);
	}else{
		return -1;
	}

	return 0;
}

int main(int argc, char *argv[])
{
	int sfd;

	int i = 0;
	int ret;

	struct sockaddr_in heraddr;
	struct icmphdr *icmp_s, *icmp_r;

	int recv_len = sizeof(struct iphdr) + sizeof(struct icmphdr) + sizeof(struct timespec);
	int send_len = sizeof(struct icmphdr) + sizeof(struct timespec);

	icmp_s = malloc(send_len);
	icmp_r = malloc(recv_len);
	if(!icmp_s || !icmp_r){
		perror("malloc");
		exit(1);
	}

	memset(icmp_s, 0, send_len);
	memset(icmp_r, 0, recv_len);

	sfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);	
	if(sfd < 0){
		perror("socket");
		exit(1);
	}

	heraddr.sin_family = AF_INET;
	heraddr.sin_port = htons(12134);
	heraddr.sin_addr.s_addr = inet_addr(argv[1]);

	while(1){
		icmp_pack(icmp_s, i++);
		ret = sendto(sfd, icmp_s, send_len, 0, (struct sockaddr *)&heraddr, sizeof(struct sockaddr));	
		if(ret < 0){
			perror("sendto");
			exit(1);
		}
		again:
		ret = recvfrom(sfd, icmp_r, recv_len, 0, NULL, NULL);
		if(ret < 0){
			perror("sendto");
			exit(1);
		}
		ret = icmp_unpack(icmp_r);
		if(ret < 0){
			goto again;
		}

		sleep(1);
	}

	close(sfd);

	return 0;
}













