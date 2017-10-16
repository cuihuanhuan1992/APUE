#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define JOBTHREAD 5

pthread_barrier_t barrier;

void *t1_handler(void *arg)
{
	int count = 5;
	long id = (long)arg;

	while(count--){
		printf("%ld\n", id);
		sleep(id + 1);
		pthread_barrier_wait(&barrier);
	}

	return NULL;
}

int main(void)
{
	int i;
	int ret;

	pthread_t t[JOBTHREAD];

	pthread_barrier_init(&barrier, NULL, JOBTHREAD);	

	for(i = 0; i < JOBTHREAD; i++){
		ret = pthread_create(t + i, NULL, t1_handler, (void *)(long)i);
		if(ret){
			perror("create fail\n");
			exit(1);
		}
	}
	/*if error*/

	for(i = 0; i < JOBTHREAD; i++){
		if(pthread_join(t[i], NULL) != 0)	
			fprintf(stderr, "error\n");
	}

	pthread_barrier_destroy(&barrier);

	return 0;
}







