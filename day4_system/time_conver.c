#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{
	time_t t;
	int ret;
	struct tm *tm;
	char buf[1024];

	t = time(NULL);
	if(t < 0){
		perror("time");
		exit(1);
	}

	printf("%s %lu\n", ctime(&t), t);
	
	//tm = gmtime(&t);	
	tm = localtime(&t);	

	printf("sec:%d\n", tm->tm_sec); 
        printf("min:%d\n", tm->tm_min);
        printf("hour:%d\n", tm->tm_hour); 
        printf("mday:%d\n", tm->tm_mday); 
        printf("mon:%d\n", tm->tm_mon + 1);  
        printf("year:%d\n", tm->tm_year + 1900); 
        printf("wday:%d\n", tm->tm_wday + 1); 
        printf("yday%d\n", tm->tm_yday); 
        printf("isdst:%d\n", tm->tm_isdst);

	strftime(buf, sizeof(buf), "%F %T\n", tm); 

	printf("time:%s", buf);

	return 0;
}









