#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

//pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex;

void *t1_handler(void *arg)
{
	int i;

	for(i = 0; i < 1000000; i++){
		pthread_mutex_lock(&mutex);
		(*(int *)arg)++;
		pthread_mutex_unlock(&mutex);
	}
}

void *t2_handler(void *arg)
{
	int i;

	for(i = 0; i < 1000000; i++){
		pthread_mutex_lock(&mutex);
		(*(int *)arg)++;
		pthread_mutex_unlock(&mutex);
	}
}

void *t3_handler(void *arg)
{
	int i;

	for(i = 0; i < 1000000; i++){
		pthread_mutex_lock(&mutex);
		(*(int *)arg)++;
		pthread_mutex_unlock(&mutex);
	}
}

int main(void)
{
	pthread_t t1, t2, t3;
	int no = 0;

	pthread_mutex_init(&mutex, NULL);

	pthread_create(&t1, NULL, t1_handler, &no);
	/*if error*/

	pthread_create(&t2, NULL, t2_handler, &no);
	/*if error*/

	pthread_create(&t3, NULL, t3_handler, &no);
	/*if error*/

	pthread_join(t1, NULL);	
	pthread_join(t2, NULL);	
	pthread_join(t3, NULL);	

	pthread_mutex_destroy(&mutex);

	printf("no = %d\n", no);

	return 0;
}







