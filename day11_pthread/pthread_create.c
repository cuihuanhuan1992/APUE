#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <fcntl.h>

void t1_p1(void *t)
{
	printf("%p\n", t);
}

void t1_p2(void *t)
{
	printf("%p\n", t);
}


void *t1_handler(void *p)
{
	pthread_cleanup_push(t1_p1, (void *)0x11);
	pthread_cleanup_push(t1_p2, (void *)0x22);

	printf("%p, t1 %u\n", p, pthread_self());

	//exit(0);
	pthread_cleanup_pop(0);

	pthread_exit((void *)1);

	pthread_cleanup_pop(1);

	return (void *)1;
}

void *t2_handler(void *p)
{
	pthread_cleanup_push(t1_p1, (void *)0x55);
	pthread_cleanup_push(t1_p2, (void *)0x66);
	sleep(1);
	printf("%p t2 %u\n", p, pthread_self());
	pthread_cleanup_pop(1);
	pthread_cleanup_pop(1);

	return (void *)2;
}

void *t3_handler(void *p)
{
	pthread_cleanup_push(t1_p1, (void *)0x33);
	pthread_cleanup_push(t1_p2, (void *)0x44);
	//sleep(1);
	printf("%p t3 %u\n", p, pthread_self());

	return (void *)3;

	pthread_cleanup_pop(1);
	pthread_cleanup_pop(1);

	//return (void *)3;
}

int main(void)
{
	char *p1, *p2, *p3;
	pthread_t tid1, tid2, tid3;

	pthread_create(&tid1, NULL, t1_handler, (void *)1);
	/*if error*/	
	pthread_create(&tid2, NULL, t2_handler, (void *)1);
	/*if error*/	
	pthread_create(&tid3, NULL, t3_handler, (void *)1);
	/*if error*/	

	pthread_detach(tid3);

	pthread_cancel(tid2);

	pthread_join(tid1, &p1);
	printf("t1 %p\n", p1);
	pthread_join(tid2, &p2);
	printf("t2 %p\n", p2);
	pthread_join(tid3, &p3);
	printf("t3 %p\n", p3);

	return 0;
}






