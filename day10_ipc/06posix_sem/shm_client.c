#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/sem.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>

#define PRO_PATH	"/home/zhiyong/apue/day10_ipc/06posix_sem/shm_client.c"
#define PRO_ID		123

#define SEM_WAIT	"/server.c"
#define SEM_COMP	"/client.c"

int main(void)
{
	int shm_id;
	key_t key;
	int *shm_v;
	int i = 0;
	int process;

	sem_t *sem_w, *sem_c;

	sem_w = sem_open(SEM_WAIT, 0);
	if(sem_w == SEM_FAILED)
		goto sem_open_wait_error;

	sem_c = sem_open(SEM_COMP, 0);
	if(sem_c == SEM_FAILED)
		goto sem_open_come_error;


	key = ftok(PRO_PATH, PRO_ID);

	shm_id = shmget(key, sizeof(int), 0);	
	if(shm_id < 0)
		goto shmget_error;

	shm_v = shmat(shm_id, NULL, SHM_RDONLY);
	if(shm_v < 0)
		goto shmat_error;

	while(1){
		if(sem_wait(sem_c) < 0)
			goto sem_wait_error;

		printf("catch start\n");
		process = *shm_v;
		printf("catch end\n");
		
		if(sem_post(sem_w) < 0)
			goto sem_post_error;

		sleep(2);
		printf("process %d is ok \n\n", process);
	}	

	return 0;
sem_post_error:
sem_wait_error:
	shmdt(shm_v);
shmat_error:
shmget_error:
	sem_close(sem_c);
sem_open_come_error:
	sem_close(sem_w);
sem_open_wait_error:
	perror("abc");
	exit(1);
}
