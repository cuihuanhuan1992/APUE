#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	char tmpdir[64] = "/tmp/tmpXXXXXX";
	char tmpfile[64];
	
	char *td;
	int fd;

	td = mkdtemp(tmpdir);
	if(!td){
		perror("mkdtemp");
		exit(1);
	}

	printf("tmpdir:%s\n", td);

	sprintf(tmpfile, "%s/tmpXXXXXX", td);

	fd = mkstemp(tmpfile);
	if(fd < 0){
		perror("mkstemp");
		exit(1);
	}

	write(fd, tmpfile, strlen(tmpfile));

	close(fd);	
	
	return 0;
}
