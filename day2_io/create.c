#include <unistd.h>
#include <stdlib.h>

int main(void)
{
	int fd;

	fd = creat("./creat.txt", 0000);
	if(fd < 0){
		perror("creat");
		exit(1);
	}

	write(fd, "nihao", 5);

	close(fd);

	return 0;
}
