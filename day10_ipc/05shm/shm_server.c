#include <stdio.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h> 
#include <sys/types.h> 
#include <sys/ipc.h>

#define PRO_PATH	"/home/zhiyong/apue/day10_ipc/05shm/shm_server.c"
#define PRO_ID		123

union semun {
	int              val;    /* Value for SETVAL */
	struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
	unsigned short  *array;  /* Array for GETALL, SETALL */
	struct seminfo  *__buf;  /* Buffer for IPC_INFO*/
};

int main(void)
{
	int shm_id;
	key_t key;
	int *shm_v;
	int i = 0;
	unsigned short arr[2] = {0}; 

	union semun sem;
	int sem_id;
	struct sembuf sops[2];

	key = ftok(PRO_PATH, PRO_ID);

	sem_id = semget(key, 2, IPC_CREAT | 0660);
	if(sem_id < 0){
		perror("server semget");
		exit(1);
	}

	sem.array = arr;
	semctl(sem_id, 0, SETALL, sem);
	/*if error*/

	shm_id = shmget(key, sizeof(int), IPC_CREAT | 0660);		
	if(shm_id < 0){
		perror("shmget");
		semctl(sem_id, 0, IPC_RMID);
		exit(1);
	}

	shm_v = shmat(shm_id, NULL, 0);
	if(shm_v < 0){
		perror("server shmat");
		shmctl(shm_id, IPC_RMID, NULL);
	}

	sops[0].sem_num = 0;
	sops[0].sem_op = 1;
	sops[0].sem_flg = 0;//SEM_UNDO;

	sops[1].sem_num = 1;
	sops[1].sem_op = -1;
	sops[1].sem_flg = 0;//SEM_UNDO;

	while(1){
		if(i == 0x7fff)
			i = 0;
		*shm_v = i++;
		printf("process id = %d\n", *shm_v);
		semop(sem_id, sops, 1);//tell client
		semop(sem_id, sops + 1, 1); //wait for client
		//sleep(1);
	}

	shmctl(shm_id, IPC_RMID, NULL);

	return 0;
}
