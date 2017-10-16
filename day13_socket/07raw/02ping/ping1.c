#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>

#include <netinet/ip_icmp.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */

//#include <linux/icmp.h>

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
	int sfd;

	int i = 0;
	int ret;

	struct sockaddr_in heraddr;
	struct icmphdr icmp_s, icmp_r;
	//struct icmp icmp_s;

	sfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);	
	if(sfd < 0){
		perror("socket");
		exit(1);
	}

	heraddr.sin_family = AF_INET;
	heraddr.sin_port = htons(12134);
	heraddr.sin_addr.s_addr = inet_addr(argv[1]);

	icmp_s.type = 8;
	icmp_s.code = 0;
	icmp_s.checksum = 0;
	icmp_s.un.echo.id = htons(getpid());
	icmp_s.un.echo.sequence = htons(i++);
	
	icmp_s.checksum = check_sum((void *)&icmp_s, sizeof(struct icmphdr));

	while(1){
		ret = sendto(sfd, &icmp_s, sizeof(struct icmphdr), 0, (struct sockaddr *)&heraddr, sizeof(heraddr));	
		if(ret < 0){
			perror("sendto");
			exit(1);
		}

		printf("send %d %d\n", i, (int)sizeof(struct icmphdr));
		icmp_s.checksum = 0;
		icmp_s.un.echo.sequence = htons(i++);
		icmp_s.checksum = check_sum((void *)&icmp_s, sizeof(icmp_s));

		sleep(1);
	}

	close(sfd);

	return 0;
}













