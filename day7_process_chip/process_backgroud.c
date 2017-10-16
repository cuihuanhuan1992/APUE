#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	char buf[1024] = "nihao";
	int ret = 5;

	while(1){
		//ret = read(0, buf, sizeof(buf));
		//if(ret < 0){
		//	perror("read");
		//	exit(1);
		//}
		//buf[ret] = 0;
		ret = write(1, buf, ret);
		if(ret < 0){
			perror("write");
			exit(1);
		}
	}

	return 0;
}

