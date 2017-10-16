#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int flag = 0;
int flag1 = 0;

void *t2_handler(void *arg)
{
	while(1){
		pthread_mutex_lock(&mutex);
		while(flag1 != 1)
		//if(flag1 != 1)
			pthread_cond_wait(&cond, &mutex);
		printf("%ld cache a xxx job %d\n", (long)arg, flag1);
		flag1 = 0;
		pthread_mutex_unlock(&mutex);
		sleep(2);
		//printf("xxx job ok %ld\n",  (long)arg);
	}
}

void *t1_handler(void *arg)
{
	while(1){
		pthread_mutex_lock(&mutex);
		while(flag != 1)
		if(flag != 1)
			pthread_cond_wait(&cond, &mutex);
		printf("%ld cache a job %d\n", (long)arg, flag);
		flag = 0;
		pthread_mutex_unlock(&mutex);
		sleep(2);
		//printf("job ok %ld\n",  (long)arg);
	}
}

void *t5_handler(void *arg)
{
	while(1){
		pthread_mutex_lock(&mutex);
		if(flag == 0)
			flag = 1;
		if(flag1 == 0)
			flag1 = 1;
		pthread_mutex_unlock(&mutex);

		//pthread_cond_signal(&cond);
		pthread_cond_broadcast(&cond);
		sleep(1);
	}
}

int main(void)
{
	pthread_t t1, t2, t3, t4, t5;

	pthread_create(&t1, NULL, t1_handler, (void *)1);
	pthread_create(&t2, NULL, t1_handler, (void *)2);
	pthread_create(&t3, NULL, t2_handler, (void *)3);
	pthread_create(&t4, NULL, t2_handler, (void *)4);
	/*if error*/

	sleep(1);
	
	pthread_create(&t5, NULL, t5_handler, NULL);
	/*if error*/

	pthread_join(t1, NULL);	
	pthread_join(t2, NULL);	
	pthread_join(t3, NULL);	
	pthread_join(t4, NULL);	
	pthread_join(t5, NULL);	

	return 0;
}







