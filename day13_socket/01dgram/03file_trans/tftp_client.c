#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */

#define TFTP_RRQ        1
#define TFTP_WRQ        2
#define TFTP_DATA       3
#define TFTP_ACK        4
#define TFTP_ERROR      5
#define TFTP_OACK       6

#define SERVER_PORT	69
#define RECVBUF_SIZE	2048

static int sfd;
static int fd;
static int end_flag = 0;
static int recv_len = 0;
static char server_ip[64];
static unsigned short server_port;

int store_data(char *buf, int len, int no)
{
	int ret;

	ret = pwrite(fd, buf, len, (no - 1) * 512);
	if(ret < 0){
		perror("pwrite");
		return -1;
	}

	if(len < 512){
		end_flag = 1;
	}

	recv_len += len;

	return 0;
}

int tftp_recv(char *buf)
{
	int ret;
	struct sockaddr_in heraddr;
	socklen_t addr_len = sizeof(heraddr);	

	ret = recvfrom(sfd, buf, RECVBUF_SIZE, 0, (struct sockaddr *)&heraddr, &addr_len);
	if(ret < 0){
		perror("recvfrom");
		return -1;
	}

	server_port = ntohs(heraddr.sin_port);

	return ret;
}

int send_ack(short no)
{
	unsigned short buf[4];
	int ret;
	struct sockaddr_in heraddr;

	heraddr.sin_family = AF_INET;
        heraddr.sin_port = htons(server_port);
        heraddr.sin_addr.s_addr = inet_addr(server_ip);

	buf[0] = htons(TFTP_ACK);
	buf[1] = htons(no);

	ret = sendto(sfd, buf, 4, 0, (const struct sockaddr *)&heraddr, sizeof(heraddr));
        if(ret < 0){
                perror("sendto");
                return -1;
        }

	return 0;
}

int tftp_unpack(char *buf, int len)
{
	unsigned short request = ntohs(*(unsigned short *)buf);
	unsigned short no = ntohs(*(unsigned short *)(buf + 2));
	int ret;

	switch(request){
		case TFTP_DATA:
			ret = store_data(buf + 4, len - 4, no);
			if(ret < 0){
				perror("store_data");
				return -1;
			}
			ret = send_ack(no);
			if(ret < 0){
				perror("send_ack");
				return -1;
			}
			break;
		case TFTP_ERROR:
			fprintf(stderr, "file error\n");
			break;
		case TFTP_OACK:
			ret = send_ack(0);
			if(ret < 0){
				perror("send_ack");
				return -1;
			}
			break;
		default:
			break;
	}
}

int tftp_request(unsigned short request, char *file_name)
{
	char buf[512];
	struct sockaddr_in heraddr;
	int request_msg_len = 0;
	int ret;

	*(unsigned short *)buf = htons(request);
	request_msg_len += 2;

	memcpy(buf + request_msg_len, file_name, strlen(file_name) + 1);
	request_msg_len += strlen(file_name) + 1;

	memcpy(buf + request_msg_len, "octet", 6);
	request_msg_len += 6;

//	memcpy(buf + request_msg_len, "timeout", 8);
//	request_msg_len += 8;
//
//	memcpy(buf + request_msg_len, "5", 2);	
//	request_msg_len += 2;

	heraddr.sin_family = AF_INET;
	heraddr.sin_port = htons(SERVER_PORT);
	heraddr.sin_addr.s_addr = inet_addr(server_ip);

	ret = sendto(sfd, buf, request_msg_len, 0, (const struct sockaddr *)&heraddr, sizeof(heraddr));	
	if(ret < 0){
		perror("sendto");
		return -1;
	}

	return 0;
}

int main(int argc, char *argv[])
{
	char recv_buf[RECVBUF_SIZE];
	int ret;

	sfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sfd < 0){
		perror("socket");
		exit(1);
	}

	fd = open(argv[2], O_CREAT | O_EXCL | O_RDWR, 0666);
	if(fd < 0){
		perror("open");
		exit(1);
	}

	memcpy(server_ip, argv[1], strlen(argv[1]) + 1);

	ret = tftp_request(TFTP_RRQ, argv[2]);
	if(ret < 0){
		perror("tftp_request");
		exit(1);
	}

	while(end_flag == 0){
		ret = tftp_recv(recv_buf);
		if(ret < 0){
			perror("tftp_recv");
			exit(1);
		}
		
		ret = tftp_unpack(recv_buf, ret);
		if(ret < 0){
			perror("tftp_unpack");
			exit(1);
		}
	}

	close(fd);
	close(sfd);
	
	printf("FILE SIZE %d\n", recv_len);

	return 0;
}










