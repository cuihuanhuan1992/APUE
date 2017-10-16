#include <stdio.h>
#include <stdlib.h>

struct student {
	int age;
	char name[64];
	int chiness;
	int math;
	int english;
};

void show_student(struct student *s)
{
	printf("name = %s\n", s->name);
	printf("age = %d\n", s->age);
	printf("chiness = %d\n", s->chiness);
	printf("math = %d\n", s->math);
	printf("english = %d\n", s->english);
}

int main(void)
{
	struct student s[3];
	struct student s1[3];
	FILE *file;
	int i;
	size_t ret;

	file = fopen("./fwrite.txt", "w+");
	if(!file){
		perror("fopen");
		exit(1);
	}	

	for(i = 0; i < 3; i++){
		s[i].age = i + 15;
		s[i].chiness = i + 100;
		s[i].math = i + 102;
		s[i].english = i + 150;	
		sprintf(s[i].name, "tom%d", i);
		ret = fwrite(&s[i], sizeof(struct student), 1, file);
		if(ret != 1){
			fprintf(stderr, "fwrite error\n");
			exit(1);
		}
	}

	rewind(file);

	for(i = 0; i < 3; i++){
		ret = fread(&s1[i], sizeof(struct student), 1, file);
		if(ret != 1){
			if(feof(file))
				fprintf(stderr, "fread eof %d\n", i);
			else
				fprintf(stderr, "fread error %d\n", i);
			exit(1);
		}
	}	

	show_student(s1);
	show_student(s1 + 1);
	show_student(s1 + 2);

	fclose(file);

	return 0;	
}
















