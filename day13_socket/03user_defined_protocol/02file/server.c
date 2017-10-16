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

int trans_file(int rfd, char *file_name)
{
	struct respond_st *d;
	int fd;
	int ret;
	char buf[1024];
	
	strcpy(buf, "/tmp/");
	strcat(buf, file_name);

	printf("start trans file %s\n", buf);

	fd = open(buf, O_RDONLY);
	if(fd < 0){
		perror("open");
		return -1;
	}

	d = malloc(sizeof(struct respond_st));
	if(!d){
		perror("malloc");
		return -1;
	}
	
	while(1){
		ret = read(fd, d->content, NAME_SIZE);		
		if(ret < 0){
			perror("read");
			return -1;
		}

		d->len = ret;
		d->respond = RESPOND_OK;

		ret = write(rfd, (void *)d, sizeof(struct respond_st));		
		if(ret < 0){
			perror("write");
			return -1;
		}		
		
		if(d->len == 0){
			break;
		}
	}

	printf("trans file %s ok\n", buf);
	
	close(fd);
	free(d);
}

int main(int argc, char *argv[])
{
	int i;
	int rfd;
	int ret;
	struct request_st *r;

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

	r = malloc(sizeof(struct request_st));
	if(!r){
		perror("malloc");
		exit(1);
	}

	for(;;){
		rfd = accept(sfd, NULL, NULL);
		if(rfd < 0){
			perror("accept");
			exit(1);
		}

		ret = readn(rfd, (void *)r, sizeof(struct request_st));
		if(ret < 0){
			perror("writen");
			exit(1);
		}

		if(ret == 0){
			break;
		}

		switch(r->request){
			case REQUEST_R:
				ret = trans_file(rfd, r->content);
				if(ret < 0){
					perror("trans_file");
					exit(1);
				}
				break;
			case REQUEST_W:
				break;
			default:
				break;
		}
		close(rfd);
	}

	free(r);

	close(rfd);
	close(sfd);

	return 0;
}






