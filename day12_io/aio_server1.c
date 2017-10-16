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
	#define	WPIPE_CLOSE	3
	int state;
	char buf[BUFSIZ];
	struct aiocb a;
};

struct myaio_st myaio[FIFO_NUM];

int main(int argc, char *argv[])
{
	int i;
	int ret;
	unsigned long flags;
	int fifo[FIFO_NUM];
	char *pfifo[FIFO_NUM] = {"/tmp/myfifo0", "/tmp/myfifo1", "/tmp/myfifo2", "/tmp/myfifo3"};
	const struct aiocb const *aiocb_list[FIFO_NUM];


	//启动四个异步IO控制块来读取四个管道

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
		/*这里需要注意的是：如果fifo是非阻塞打开的，即使打开后又设置了阻塞，那么如果写端没打开，读端会返回0（不会阻塞, 和写端关闭是一样的）*/

		myaio[i].state = EMPTY;
		myaio[i].a.aio_fildes = fifo[i];
		myaio[i].a.aio_offset = 0;
		myaio[i].a.aio_buf = myaio[i].buf;
		myaio[i].a.aio_nbytes = BUFSIZ;
		myaio[i].a.aio_sigevent.sigev_notify = SIGEV_NONE;
	}

	printf("ok\n");

	while(1){
		for(i = 0; i < FIFO_NUM; i++){
			switch(myaio[i].state){
				case EMPTY:
					myaio[i].state = READING;
					myaio[i].a.aio_fildes = fifo[i];
					myaio[i].a.aio_offset = 0;
					myaio[i].a.aio_buf = myaio[i].buf;
					myaio[i].a.aio_nbytes = BUFSIZ;

					ret = aio_read(&myaio[i].a);
					if(ret < 0){
						perror("aio_read empty");
						continue;
					}
				
					aiocb_list[i] = &myaio[i].a;
					break;
				case READING:
					ret = aio_error(&myaio[i].a);	
					if(ret == 0){
						if(aio_return(&myaio[i].a) == 0){
							myaio[i].state = WPIPE_CLOSE;
							fprintf(stderr, "fifo %d write port close\n", i);
							break;
						}
							
						myaio[i].state = WRITING;
						myaio[i].a.aio_fildes = 1;
						myaio[i].a.aio_offset = 0;
						myaio[i].a.aio_buf = myaio[i].buf;
						myaio[i].a.aio_nbytes = aio_return(&myaio[i].a);
						
						ret = aio_write(&myaio[i].a);
						if(ret < 0){
							perror("aio_read empty");
							continue;
						}
							
						aiocb_list[i] = &myaio[i].a;
					}
					break;
				case WRITING:
					ret = aio_error(&myaio[i].a);
                                        if(ret == 0){
                                                myaio[i].state = EMPTY;
						aiocb_list[i] = &myaio[i].a;
                                        }

					break;
				default:
					break;
			}
				
		}
		ret = aio_suspend(aiocb_list, FIFO_NUM, NULL);
		if(ret < 0){
			fprintf(stderr, "aio_suspend error\n");
		}
	}

	for(i = 0; i < FIFO_NUM; i++){
		close(fifo[i]);
	}

	return 0;
}
