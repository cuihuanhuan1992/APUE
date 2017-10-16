#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <aio.h>

#define MYPORT	1234
#define HERPORT	2234

#define BUFSIZE	1024

struct sock_aio {
	char buf[BUFSIZE];
	struct aiocb aio;
};

void thread_handler(union sigval s)
{
	int ret;
	struct sock_aio *a = s.sival_ptr;
	
	ret = aio_error(&a->aio);
	if(ret){
		perror("aio_read error");
	}else{
		ret = aio_return(&a->aio);
		if(ret < 0){
			perror("aio_return");
		}else{
			printf("\nfrom her:%s\nPlease input> ", a->buf);
			fflush(stdout);
		}
	}

	ret = aio_read(&a->aio);
	if(ret < 0){
		perror("aio_read thread");
		exit(1);
	}
}

struct sock_aio sa;	

int main(int argc, char *argv[])
{
	int sfd;
	size_t len;
	int ret;
	struct sockaddr_in myaddr, heraddr;
	char *content = NULL;

	sfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sfd < 0){
		perror("socket");
		exit(1);
	}

	heraddr.sin_family = AF_INET;
	heraddr.sin_port = htons(HERPORT);
	inet_pton(AF_INET, argv[1], &heraddr.sin_addr);

	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(MYPORT);
	myaddr.sin_addr.s_addr = INADDR_ANY;

	ret = bind(sfd, (const struct sockaddr *)&myaddr, sizeof(myaddr));
	if(ret < 0){
		perror("bind");
		exit(1);
	}

	sa.aio.aio_fildes = sfd;  	/* File descriptor */
	sa.aio.aio_offset = 0;     		/* File offset */
	sa.aio.aio_buf = sa.buf;        	/* Location of buffer */
	sa.aio.aio_nbytes = BUFSIZE;     	/* Length of transfer */
	sa.aio.aio_sigevent.sigev_notify = SIGEV_THREAD;   		/* Notification method */
	sa.aio.aio_sigevent.sigev_value.sival_ptr = &sa;   		/* Notification method */
	sa.aio.aio_sigevent.sigev_notify_function = thread_handler;

	ret = aio_read(&sa.aio);
	if(ret < 0){
		perror("aio_read");
		exit(1);
	}
	
	while(1){
		printf("Please input> ");
		fflush(stdout);
		ret = getline(&content, &len, stdin);	
		if(ret < 0){
			perror("getline");
			continue;
		}
		ret = sendto(sfd, content, ret + 1, 0, (const struct sockaddr *)&heraddr, sizeof(heraddr));
		if(ret < 0){
			perror("send msg error");
			continue;
		}
		printf("to her:%s", content);
	}	

	free(content);
	close(sfd);

	return 0;
}















