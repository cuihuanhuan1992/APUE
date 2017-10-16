#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <time.h>

int main(void)
{
	pid_t pid;
	time_t start, end;
	long long count = 0;
	int n;

	start = time(NULL);

	pid = fork();
	if(pid < 0){
		perror("fork");
		exit(1);
	}else if(pid == 0){
		printf("child_nice = %d\t cpu_id = %d\n", getpriority(PRIO_PROCESS, 0), sched_getcpu());
		errno = 0;
		//n = nice(10);
		n = setpriority(PRIO_PROCESS, 0, 10);
		if(n == -1 && errno != 0){
			perror("nice");
			exit(1);
		}
		printf("child_nice = %d\n", nice(0));
		while(1){
			count++;
			end = time(NULL);
			if(end > start + 30){
				printf("child:%ld\n", count);
				exit(0);
			}
		}
	}

	printf("father_nice = %d\t cpu_id = %d\n", nice(0), sched_getcpu());
	while(1){
		count++;
		end = time(NULL);
		if(end > start + 30){
			printf("father:%ld\n", count);
			exit(0);
		}
	}

	return 0;
}



















