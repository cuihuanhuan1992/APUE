#include <stdio.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h> 
#include <fcntl.h>
#include <semaphore.h>
#include <sys/types.h> 
#include <sys/ipc.h>

#define SEM_WAIT	"/server.c"
#define SEM_COMP	"/client.c"

#define PRO_PATH	"/home/zhiyong/apue/day10_ipc/06posix_sem/shm_client.c"
#define PRO_ID		123	

int main(void)
{
	int shm_id;
	key_t key;
	int *shm_v;
	int i = 0;
	sem_t *sem_w, *sem_c;

	sem_w = sem_open(SEM_WAIT, O_CREAT, 0666, 0);
	if(sem_w == SEM_FAILED)
		goto sem_open_wait_error;

	sem_c = sem_open(SEM_COMP, O_CREAT, 0666, 0);
	if(sem_c == SEM_FAILED)
		goto sem_open_come_error;

	key = ftok(PRO_PATH, PRO_ID);

	shm_id = shmget(key, sizeof(int), IPC_CREAT | 0660);		
	if(shm_id < 0)
		goto shmget_error;

	shm_v = shmat(shm_id, NULL, 0);
	if(shm_v < 0)
		goto shmat_error;

	while(1){
		if(i == 0x7fff)
			i = 0;
		*shm_v = i++;
		printf("process id = %d\n", *shm_v);
		sem_post(sem_c);//tell client
		sem_wait(sem_w); //wait for client
		//sleep(1);
	}

	
	shmctl(shm_id, IPC_RMID, NULL);

	sem_close(sem_w);
	sem_close(sem_c);

	sem_unlink(SEM_WAIT);
        sem_unlink(SEM_COMP);

	return 0;
shmat_error:
	shmctl(shm_id, IPC_RMID, NULL);
shmget_error:
	sem_close(sem_c);
	sem_unlink(SEM_COMP);
sem_open_come_error:
	sem_close(sem_w);
	sem_unlink(SEM_WAIT);
sem_open_wait_error:
	perror("");
	exit(1);
}





