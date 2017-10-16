#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

int source = 0;

void *t1_handler(void *arg)
{
	int i;
	int ret;

	printf("1\n");

	for(i = 0; i < 1000000; i++){
		pthread_mutex_lock(&mutex1);
		source++;

		//pthread_mutex_lock(&mutex);
		//(*(int *)arg)++;
		//pthread_mutex_unlock(&mutex);

		//again:
		//ret = pthread_mutex_trylock(&mutex);
		//if(ret == 0){
		//	(*(int *)arg)++;
		//	pthread_mutex_unlock(&mutex);
		//}else{
		//	pthread_mutex_unlock(&mutex1);
		//	sleep(1);
		//	pthread_mutex_lock(&mutex1);
		//	goto again;
		//}

		again:
		//注意：如果枷锁不成功的话是返回错误码，不是小于零的数组
		ret = pthread_mutex_trylock(&mutex);
		if(ret != 0){
			printf("fail to lock mutex %d\n", ret);
			pthread_mutex_unlock(&mutex1);
			sleep(1);
			pthread_mutex_lock(&mutex1);
			goto again;
		}

		(*(int *)arg)++;
		pthread_mutex_unlock(&mutex);

		pthread_mutex_unlock(&mutex1);
	}
}

void *t2_handler(void *arg)
{
	int i;

	printf("2\n");

	for(i = 0; i < 1000000; i++){
		pthread_mutex_lock(&mutex);
		(*(int *)arg)++;

		pthread_mutex_lock(&mutex1);
		source++;
		pthread_mutex_unlock(&mutex1);

		pthread_mutex_unlock(&mutex);
	}
}

void *t3_handler(void *arg)
{
	int i;

	printf("3\n");

	for(i = 0; i < 1000000; i++){
		pthread_mutex_lock(&mutex);
		(*(int *)arg)++;

		pthread_mutex_lock(&mutex1);
		source++;
		pthread_mutex_unlock(&mutex1);

		pthread_mutex_unlock(&mutex);
	}
}

int main(void)
{
	pthread_t t1, t2, t3;
	int no = 0;

	pthread_create(&t1, NULL, t1_handler, &no);
	/*if error*/
	
	pthread_create(&t2, NULL, t2_handler, &no);
	/*if error*/

	pthread_create(&t3, NULL, t3_handler, &no);
	/*if error*/

	pthread_join(t1, NULL);	
	pthread_join(t2, NULL);	
	pthread_join(t3, NULL);	


	printf("no = %d\n", no);
	printf("source = %d\n", source);

	return 0;
}







