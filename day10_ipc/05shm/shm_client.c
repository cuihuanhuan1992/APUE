#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>

#define PRO_PATH	"/home/zhiyong/apue/day10_ipc/05shm/shm_server.c"
#define PRO_ID		123

int main(void)
{
	int shm_id;
	key_t key;
	int *shm_v;
	int i = 0;
	int process;

	int sem_id;
	struct sembuf sops[2];

	key = ftok(PRO_PATH, PRO_ID);

	sem_id = semget(key, 0, IPC_CREAT | 0660);
	/*if error*/
	
	shm_id = shmget(key, sizeof(int), IPC_CREAT | 0660);		
	if(shm_id < 0){
		perror("shmget");
		exit(1);
	}

	shm_v = shmat(shm_id, NULL, SHM_RDONLY);
	if(shm_v < 0){
		perror("client shmat");
		shmctl(shm_id, IPC_RMID, NULL);
	}

	sops[0].sem_num = 0;
        sops[0].sem_op = -1;
        sops[0].sem_flg = 0;//SEM_UNDO;

	sops[1].sem_num = 1;
        sops[1].sem_op = 1;
        sops[1].sem_flg = 0;//SEM_UNDO;

	while(1){
		if(semop(sem_id, sops, 1) < 0){
			perror("client semop");
			exit(1);
		}
		/*if error*/
		printf("catch start\n");
		process = *shm_v;
		printf("catch end\n");
		
		if(semop(sem_id, sops + 1, 1) < 0){
			perror("client semop end");
			exit(1);
		}
		sleep(2);
		printf("process %d is ok \n\n", process);
	}	

	return 0;
}
