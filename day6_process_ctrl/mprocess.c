#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int main(void)
{
	int fd;
	pid_t pid;
	char buf[32];
	int ret;
	int count = 100000;

	fd = open("./mp.txt", O_RDWR | O_CREAT | O_TRUNC, 0660);	
	if(fd < 0){
		perror("open mp.txt");
		exit(1);
	}

	write(fd, "0", 1);
	/*if error*/

	pid = fork();
	if(pid < 0){
		perror("fork");
		exit(1);
	}else if(pid == 0){
		while(count--){
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
		}	
		exit(0);
	}

	while(count--){
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
	}	

	return 0;
}
