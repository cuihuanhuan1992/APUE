#include <stdio.h>
#include <stdlib.h>
#include <aio.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <unistd.h>

#define FIFO_NUM	4

struct myaio_st {
	#define EMPTY		0
	#define WRITING		1
	#define READING		2
	int state;
	char buf[BUFSIZ];
	struct aiocb a;
};

struct myaio_st myaio[FIFO_NUM];

void thread_handler(union sigval s)
{
	int ret;
	int no = s.sival_int;
	
	ret = aio_return(&myaio[no].a);	
	if(ret < 0){
		fprintf(stderr, "fifo %d read error\n", no);
	}

	if(ret == 0){
		fprintf(stderr, "fifo %d write port close\n", no);
		return;
	}

	if(ret > 0){
		ret = write(1, myaio[no].buf, ret);
		if(ret < 0){
			fprintf(stderr, "data from fifo %d write error\n", no);
		}
		write(1, "\n", 1);
	}

	ret = aio_read(&myaio[no].a);
	if(ret < 0){
		perror("aio_read");
		exit(1);
	}
}

int main(int argc, char *argv[])
{
	int i;
	int ret;
	int num;
	unsigned long flags;
	int fifo[FIFO_NUM];
	char *pfifo[FIFO_NUM] = {"/tmp/myfifo0", "/tmp/myfifo1", "/tmp/myfifo2", "/tmp/myfifo3"};

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

		flags &= ~O_NONBLOCK;

		ret = fcntl(fifo[i], F_SETFL, flags);
		if(ret < 0){
			perror("fcntl1");
			exit(1);
		}
		/*这里需要注意的是：如果fifo是非阻塞打开的，即使后来又设置了阻塞，那么如果写端没打开，读端会返回0（不会阻塞, 和写端关闭是一样的）*/

		myaio[i].state = EMPTY;
		myaio[i].a.aio_fildes = fifo[i];
		myaio[i].a.aio_offset = 0;
		myaio[i].a.aio_buf = myaio[i].buf;
		myaio[i].a.aio_nbytes = BUFSIZ;
		myaio[i].a.aio_sigevent.sigev_notify = SIGEV_THREAD;
		myaio[i].a.aio_sigevent.sigev_value.sival_int = i;
		myaio[i].a.aio_sigevent.sigev_notify_function = thread_handler;

		ret = aio_read(&myaio[i].a);
		if(ret < 0){
			perror("aio_read");
			exit(1);
		}
	}

	printf("ok\n");

	pause();

	for(i = 0; i < FIFO_NUM; i++){
		close(fifo[i]);
	}

	return 0;
}
