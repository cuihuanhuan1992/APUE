#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{
	time_t t;
	int ret;
	struct timespec times;

	t = time(NULL);
	if(t < 0){
		perror("time");
		exit(1);
	}

	printf("%s %lu\n", ctime(&t), t);

	ret = clock_getres(CLOCK_MONOTONIC, &times);
	if(ret < 0){
		perror("clock_getres");
		exit(1);
	}

	printf("精度：%lus\n%luns\n", times.tv_sec, times.tv_nsec);

	ret = clock_gettime(CLOCK_MONOTONIC, &times);
	if(ret < 0){
		perror("clock_gettime");
		exit(1);
	}

	printf("时间：%lus\n%luns\n", times.tv_sec, times.tv_nsec);

	ret = clock_gettime(CLOCK_REALTIME, &times);
	if(ret < 0){
		perror("clock_gettime");
		exit(1);
	}

	printf("实时时间：%lus\n%luns\n", times.tv_sec, times.tv_nsec);

	times.tv_sec = 0;
	times.tv_nsec = 0;

	ret = clock_settime(CLOCK_REALTIME, &times);
        if(ret < 0){
                perror("clock_gettime");
                exit(1);
        }

        printf("设置过的实时时间：%lus\n%luns\n", times.tv_sec, times.tv_nsec);

	return 0;
}









