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
	int ret;
	char *name[STUDENT_NUM] = {"zhangsan", "lisi", "wangwu", "zhangliu"};
	int age[STUDENT_NUM] = {16, 17, 18, 19};
	int chiness[STUDENT_NUM] = {116, 117, 118, 119};
	int english[STUDENT_NUM] = {1116, 1117, 1118, 1119};
	struct student_st *s;

	struct sockaddr_in heraddr;

	int sfd;

	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sfd < 0){
		perror("socket");
		exit(1);
	}

	heraddr.sin_family = AF_INET;
	heraddr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, argv[1], &heraddr.sin_addr);

	ret = connect(sfd, (const struct sockaddr *)&heraddr, sizeof(heraddr));
	if(ret < 0){
		perror("connect");
		exit(1);
	}

	for(i = 0; i < STUDENT_NUM; i++){
		s = malloc(sizeof(struct student_st));
		if(!s){
			perror("malloc");
			exit(1);
		}

		strcpy(s->name, name[i]);
		s->age = age[i];
		s->english = english[i];
		s->chiness = chiness[i];

		ret = writen(sfd, (void *)s, sizeof(struct student_st));
		if(ret < 0){
			perror("writen");
			exit(1);
		}

		free(s);
	}

	close(sfd);

	return 0;
}






