#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/fcntl.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */

#include "protocol.h"

#define STUDENT_NUM	4

int readn(int fd, char *buf, int n)
{
        int rn = 0;
        int ret;

        while(n > 0){
                ret = read(fd, buf + rn, n);
                if(ret < 0){
                        if(rn == 0)
                                return -1;
                        else
                                break;
                }else if(ret == 0){
                        break;
                }

                rn += ret;
                n -= ret;
        }

        return rn;
}

int writen(int fd, char *buf, int n)
{
        int rn = 0;
        int ret;

        while(n > 0){
                ret = write(fd, buf + rn, n);
                if(ret < 0){
                        if(rn == 0)
                                return -1;
                        else
                                break;
                }else if(ret == 0){
                        break;
                }

                rn += ret;
                n -= ret;
        }

        return rn;
}

int main(int argc, char *argv[])
{
	int i;
	int rfd;
	int ret;
	struct student_st *s;

	struct sockaddr_in myaddr;

	int sfd;

	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sfd < 0){
		perror("socket");
		exit(1);
	}

	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, "0.0.0.0", &myaddr.sin_addr);

	ret = bind(sfd, (const struct sockaddr *)&myaddr, sizeof(myaddr));
	if(ret < 0){
		perror("bind");
		exit(1);
	}

	ret = listen(sfd, 20);
	if(ret < 0){
		perror("listen");
		exit(1);
	}

	rfd = accept(sfd, NULL, NULL);
	if(rfd < 0){
		perror("accept");
		exit(1);
	}

	for(;;){
		s = malloc(sizeof(struct student_st));
		if(!s){
			perror("malloc");
			exit(1);
		}

		ret = readn(rfd, (void *)s, sizeof(struct student_st));
		if(ret < 0){
			perror("writen");
			exit(1);
		}

		if(ret == 0){
			break;
		}

		printf("name:%s\n", s->name);
		printf("age:%d\n", s->age);
		printf("english:%d\n", s->english); 
		printf("chiness:%d\n", s->chiness);

		printf("\n");

		free(s);
	}

	close(rfd);
	close(sfd);

	return 0;
}






