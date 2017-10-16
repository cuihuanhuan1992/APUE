#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define JOBTHREAD	2
#define START		0
#define END		100000

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int job[JOBTHREAD] = {0};

void *t1_handler(void *arg)
{
	long id = (long)arg;
	int data;
	int i;

	while(1){
		pthread_mutex_lock(&mutex);
		while(job[id] == 0){
			pthread_cond_wait(&cond, &mutex);
		}

		if(job[id] == -1){
			pthread_mutex_unlock(&mutex);
			pthread_exit(NULL);
		}

		data = job[id];
		job[id] = 0;
		pthread_mutex_unlock(&mutex);
		
		for(i = 2; i < data; i++){
			if((data % i) == 0)
				break;
		}

		if(i == data)
			printf("%d is a prime %ld  %ld\n", data, id, (long)pthread_self());
	}

	return NULL;
}

void *t5_handler(void *arg)
{
	int i, j;

	for(i = START; i < END; ){
		pthread_mutex_lock(&mutex);
		for(j = 0; j < JOBTHREAD && i < END; j++){
			if(job[j] == 0)
				job[j] = i++;
		}
		pthread_mutex_unlock(&mutex);
		//pthread_cond_signal(&cond);
		pthread_cond_broadcast(&cond);
		//sleep(2);
	}

	while(1){
		pthread_mutex_lock(&mutex);
		for(j = 0; j < JOBTHREAD; j++){
			if(job[j] == 0) job[j] = -1;
		}
		pthread_mutex_unlock(&mutex);
		pthread_cond_broadcast(&cond);

		pthread_mutex_lock(&mutex);
		for(j = 0; j < JOBTHREAD; j++){
			if(job[j] != -1) break;
		}
		pthread_mutex_unlock(&mutex);
		if(j == JOBTHREAD){ 
			break;
		}
	}

	return NULL;
}

int main(void)
{
	int i;
	int ret;

	pthread_t mainthread;
	pthread_t t[JOBTHREAD];

	for(i = 0; i < JOBTHREAD; i++){
		ret = pthread_create(t + i, NULL, t1_handler, (void *)(long)i);
		if(ret){
			perror("create fail\n");
			exit(1);
		}
	}
	/*if error*/

	pthread_create(&mainthread, NULL, t5_handler, t);

	pthread_join(mainthread, NULL);	

	//for(i = 0; i < JOBTHREAD; i++){
	//	printf("cancel %ld\n", (unsigned long)t[i]);
	//	if(pthread_cancel(t[i]) != 0)
	//		fprintf(stderr, "error\n");
	//}

	for(i = 0; i < JOBTHREAD; i++){
		printf("join %ld\n", (unsigned long)t[i]);
		if(pthread_join(t[i], NULL) != 0)	
			fprintf(stderr, "error\n");
	}

	return 0;
}







