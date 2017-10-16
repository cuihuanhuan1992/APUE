#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <unistd.h>

#define FIFO_NUM	4

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
	int num;
	long flags;
	char buf[1024];
	int efd;
	int fifo[FIFO_NUM];
	char *pfifo[FIFO_NUM] = {"/tmp/myfifo0", "/tmp/myfifo1", "/tmp/myfifo2", "/tmp/myfifo3"};
	struct epoll_event ev;
	struct epoll_event rev[FIFO_NUM];

	efd = epoll_create(FIFO_NUM);
	if(efd < 0){
		perror("epoll_ceate");
		exit(1);
	}

	for(i = 0; i < FIFO_NUM; i++){
		fifo[i] = open(pfifo[i], O_RDONLY | O_NONBLOCK);
		if(fifo[i] < 0){
			perror("open fifo");
			exit(1);
		}

		flags = fcntl(fifo[i], F_GETFL, 0);
		if(flags < 0){
			perror("fcntl0");
			exit(1);
		}

		ret = fcntl(fifo[i], F_SETFL, flags & ~O_NONBLOCK);
		if(ret < 0){
			perror("fcntl1");
			exit(1);
		}

		ev.events = 0;
		ev.events |= EPOLLIN;
		ev.data.fd = fifo[i];
		ret = epoll_ctl(efd, EPOLL_CTL_ADD, fifo[i], &ev);
		if(ret < 0){
			perror("epoll_ctl");
			exit(1);
		}
	}

	while(1){
		num = epoll_wait(efd, rev, FIFO_NUM, -1);
		if(num < 0){
			perror("epoll wait");
			continue;
		} 
		for(i = 0; i < num; i++){
			ret = readn(rev[i].data.fd, buf, 100);		
			if(ret < 0){
				fprintf(stderr, "read fifo[%d] error\n", rev[i].data.fd);
				continue;
			}
			printf("ret = %d\n", ret);
			//如果读fifo返回0，说明管道写端关闭了
			if(ret == 0){
				ret = epoll_ctl(efd, EPOLL_CTL_DEL, rev[i].data.fd, &rev[i]);
				if(ret < 0){
					perror("epoll del");
					exit(1);
				}
				continue;
			}
			ret = write(1, buf, ret);
			if(ret < 0){
				fprintf(stderr, "write fifo[%d] error\n", rev[i].data.fd);
                                continue;
			}
			write(1, "\n", 1);
		}
	}	

	close(efd);
	
	for(i = 0; i < FIFO_NUM; i++){
		close(fifo[i]);
	}

	return 0;
}
