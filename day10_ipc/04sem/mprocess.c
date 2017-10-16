#include <stdio.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <stdlib.h>

#define PROJECT_PATH 	"/home/zhiyong/apue/day10_ipc/04sem/mprocess.c"
#define PROJECT_ID	100

union semun {
	int              val;    /* Value for SETVAL */
	struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
	unsigned short  *array;  /* Array for GETALL, SETALL */
	struct seminfo  *__buf;  /* Buffer for IPC_INFO*/
};

int main(void)
{
	int fd;
	pid_t pid;
	char buf[32];
	int ret;
	int count = 100000;
	key_t key;
	int sem_id;
	union semun arg;
	struct sembuf sbuf[2];

	fd = open("./mp.txt", O_RDWR | O_CREAT | O_TRUNC, 0660);	
	if(fd < 0){
		perror("open mp.txt");
		exit(1);
	}

	write(fd, "0", 1);
	/*if error*/

	key = ftok(PROJECT_PATH, PROJECT_ID);
	if(key < 0){
		perror("ftok");
		exit(1);
	}

	sem_id = semget(key, 1, IPC_CREAT | 0660);
	if(sem_id < 0){
		perror("msgget");
		exit(1);
	}

	arg.val = 1;

	ret = semctl(sem_id, 0, SETVAL, arg);
	if(ret < 0){
		semctl(sem_id, 0, IPC_RMID);
	}

	sbuf[0].sem_num = 0;
	sbuf[0].sem_op = -1;
	sbuf[0].sem_flg = SEM_UNDO;

	sbuf[1].sem_num = 0;
	sbuf[1].sem_op = 1;
	sbuf[1].sem_flg = SEM_UNDO;

	pid = fork();
	if(pid < 0){
		perror("fork");
		exit(1);
	}else if(pid == 0){
		while(count--){
			ret = semop(sem_id, sbuf, 1);		
			if(ret < 0){
				perror("semop");
				exit(1);
			}
			lseek(fd, 0, SEEK_SET);
			/*if error*/
			ret = read(fd, buf, 32);
			/*if error*/
			ret = atoi(buf) + 1;
			ret = sprintf(buf, "%d", ret);
			lseek(fd, 0, SEEK_SET);
			/*if error*/
			write(fd, buf, ret);
			/*if error*/
			ret = semop(sem_id, sbuf + 1, 1);		
			if(ret < 0){
				perror("semop");
				exit(1);
			}
		}	
		close(fd);
		exit(0);
	}

	pid = fork();
        if(pid < 0){
                perror("fork");
                exit(1);
        }else if(pid == 0){
                while(count--){
			ret = semop(sem_id, sbuf, 1);		
			if(ret < 0){
				perror("semop");
				exit(1);
			}
                        lseek(fd, 0, SEEK_SET);
                        /*if error*/
                        ret = read(fd, buf, 32);
                        /*if error*/
                        ret = atoi(buf) + 1;
                        ret = sprintf(buf, "%d", ret);
                        lseek(fd, 0, SEEK_SET);
                        /*if error*/
                        write(fd, buf, ret);
                        /*if error*/
			ret = semop(sem_id, sbuf + 1, 1);		
			if(ret < 0){
				perror("semop");
				exit(1);
			}
                }
		close(fd);
                exit(0);
        }


	while(count--){
		ret = semop(sem_id, sbuf, 1);		
		if(ret < 0){
			perror("semop");
			exit(1);
		}
		lseek(fd, 0, SEEK_SET);
		/*if error*/
		ret = read(fd, buf, 32);
		/*if error*/
		buf[ret] = 0;
		ret = atoi(buf) + 1;
		ret = sprintf(buf, "%d", ret);
		lseek(fd, 0, SEEK_SET);
		/*if error*/
		write(fd, buf, ret);
		/*if error*/
		ret = semop(sem_id, sbuf + 1, 1);		
		if(ret < 0){
			perror("semop");
			exit(1);
		}
	}	

	while(1){
		ret = wait(NULL);
		if((ret < 0) && (errno == ECHILD))
			break;
	}

	close(fd);

	ret = semctl(sem_id, 0, IPC_RMID);
	if(ret < 0){
		perror("semctl rm");
		exit(1);
	}

	return 0;
}




