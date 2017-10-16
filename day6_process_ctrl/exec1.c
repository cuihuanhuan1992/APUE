#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

extern char **environ;

int main(int argc, char *argv[])
{
	//execl("/bin/ls", "helloworld", "/", NULL);
	//execlp("ls", "haha", ".", NULL);
	//execv("/bin/ls", argv);
	
	char *buf[3];
	buf[0] = "PWD=/home/zh";
	buf[1] = "HELLO=world";
	buf[2] = "NIHAO=hehe";

	//execle("/bin/pwd", "pwd", NULL, environ);	

	execle("/usr/bin/printenv", "pwd", NULL, buf);	
	perror("execl");	

	return 0;
}
