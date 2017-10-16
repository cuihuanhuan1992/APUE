#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/times.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sched.h>
#include <time.h>
#include <sys/times.h>

int main(int argc, char *argv[])
{
	pid_t pid;
	int cps;
	clock_t start, end;
	struct tms tstart, tend;

	cps = sysconf(_SC_CLK_TCK);
	
	start = times(&tstart);
	system(argv[1]);
	end = times(&tend);

	printf("real\t%6.2f\n", (end - start) / (float)cps);
	printf("user\t%6.2f\n", (tend.tms_utime - tstart.tms_utime) / (float)cps);
	printf("sys\t%6.2f\n", (tend.tms_stime - tstart.tms_stime) / (float)cps);


	printf("child user\t%6.2f\n", (tend.tms_cutime - tstart.tms_cutime) / (float)cps);
	printf("father sys\t%6.2f\n", (tend.tms_cstime - tstart.tms_cstime) / (float)cps);
	
	return 0;
}



















