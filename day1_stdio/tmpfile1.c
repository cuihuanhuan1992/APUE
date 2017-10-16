#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	FILE *file;
	char buf[64];

	file = tmpfile();
	if(!file){
		perror("tmpfile");
		exit(1);
	}

	fwrite("hello", 5, 1, file);
	/* if error */

	fseek(file, SEEK_SET, 0);
	/* if error */

	fread(buf, 5, 1, file);
	/* if error */
	
	fwrite(buf, 5, 1, stdout);	
	/* if error */

	fclose(file);

	return 0;
}





