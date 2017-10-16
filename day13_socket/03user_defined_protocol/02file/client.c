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

int request_r(int sfd, char *file_name)
{
	struct request_st *request;
	int ret;

	request = malloc(sizeof(struct request_st));
	if(!request){
		perror("malloc");
		goto malloc_error;
	}

	request->request = REQUEST_R;
	request->options = 0;
	strcpy(request->content, file_name);

	ret = writen(sfd, (void *)request, sizeof(struct request_st));
	if(ret < 0){
		perror("writen");
		goto writen_error;
	}

	free(request);

	return ret;
writen_error:
	free(request);
malloc_error:
	return -1;
}

int store_data(int fd, char *buf, int len)
{
	int ret;

	ret = write(fd, buf, len);
	if(ret < 0){
		perror("write");
		return -1;
	}

	return ret;
}

int main(int argc, char *argv[])
{
	int i;
	int ret;
	struct request_st *s;
	struct respond_st *d;
	int fd;

	struct sockaddr_in heraddr;

	int sfd;

	fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if(fd < 0){
		perror("open");
		exit(1);
	}

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

	ret = request_r(sfd, argv[2]);
	if(ret < 0){
		perror("request_r");
		exit(1);
	}

	d = malloc(sizeof(struct respond_st));
	if(!d){
		perror("malloc");
		exit(1);
	}

	while(1){
		ret = readn(sfd, (void *)d, sizeof(struct respond_st));
		if(ret < 0){
			perror("readn");
			exit(1);
		}

		if(d->len == 0){
			break;
		}
		
		switch(d->respond){
			case RESPOND_OK:
				ret = store_data(fd, d->content, d->len); 
				if(ret < 0){
					perror("store_data");
					exit(1);
				}
				break;
			case RESPOND_ERROR:
				fprintf(stderr, "error block %d\n", d->options);
				exit(1);
				break;
			default:
				break;
		}
	}

	free(d);
	close(fd);
	close(sfd);

	return 0;
}










