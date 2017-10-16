#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <unistd.h>

int main(void)
{
	char *pfifo[4] = {"/tmp/myfifo0", "/tmp/myfifo1", "/tmp/myfifo2", "/tmp/myfifo3"};
	int i;
	int ret;

	for(i = 0; i < 4; i++){
		ret = mkfifo(pfifo[i], 0660);
		if(ret < 0){
			perror("mkfifo");
			exit(1);
		}
	}

	return 0;
}
