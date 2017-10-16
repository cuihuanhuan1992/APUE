#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <semaphore.h>

int main(void)
{
	sem_t *sem, *sem1;
	pid_t pid;

	sem = mmap(0, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if(sem < 0){
		perror("mmap");
		goto mmap_error;
	}

	sem1 = mmap(0, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if(sem < 0){
		perror("mmap");
		goto mmap1_error;
	}

	sem_init(sem, 1, 0);
	sem_init(sem1, 1, 0);

	pid = fork();
	if(pid < 0){
		perror("fork");
		goto fork_error;
	}else if(pid == 0){
		while(1){
			sem_wait(sem);	
			printf("child process start %d\n", (int)getpid());
			sleep(2);
			printf("child process end %d\n", (int)getpid());
			sem_post(sem1);
		}
	}

	while(1){
		printf("father %d\n", (int)getpid());
		sem_post(sem);	
		sem_wait(sem1);
	}

	return 0;
fork_error:
	munmap(sem1, sizeof(sem_t));
mmap1_error:
	munmap(sem, sizeof(sem_t));
mmap_error:
	exit(1);
}

