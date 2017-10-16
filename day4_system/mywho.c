#include <utmp.h>
#include <time.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
	int fd;
	struct utmp u;
	int ret;
	time_t t;

	fd = open(UTMP_FILE, O_RDONLY);
	if(fd < 0){
		perror("open");
		exit(1);
	}

	while(ret){
		ret = read(fd, &u, sizeof(struct utmp));	
		if(ret < 0){
			perror("read");
			exit(1);
		}

		t = (time_t)u.ut_tv.tv_sec;

		if(u.ut_type == USER_PROCESS)
			printf("%s\t%s\t%s", u.ut_user, u.ut_line, ctime(&t));
	}

	return 0;
}
