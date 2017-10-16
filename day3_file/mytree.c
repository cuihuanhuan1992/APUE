#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h> 

static char strbuf[4096];
static int last_dir;
static int no;

void file_style(char *file)
{
	struct stat buf;
	int ret;

	ret = stat(file, &buf);
	if(ret < 0){
		perror("stat file");
		exit(1);
	}

	switch(buf.st_mode & S_IFMT){
		case S_IFSOCK:
			printf("socket\n");
			break;
		case S_IFLNK:
			printf("symbol link\n");
			break;
		case S_IFREG:
			printf("reguler\n");
			break;
		case S_IFBLK:
			printf("block\n");
			break;
		case S_IFDIR:
			printf("dir\n");
			break;
		case S_IFCHR:
			printf("char\n");
			break;
		case S_IFIFO:
			printf("fifo\n");
			break;
		default:
			break;
	}
}

int is_dir(char *name)
{
	char dir_buf[4096];
	struct stat astat;

	strcpy(dir_buf, strbuf);
	strcat(dir_buf, "/");
	strcat(dir_buf, name);

	if(lstat(dir_buf, &astat) < 0){
		perror("lstat is_dir");
		fprintf(stderr, "%s\n", dir_buf);
		exit(1);
	}

	if(S_ISLNK(astat.st_mode)){
		return 0;//符号链接当作普通文件处理
	}

	//if(stat(dir_buf, &astat) < 0){
	//	//fprintf(stderr, "stat error %s\n", dir_buf);
	//	return 0;
	//}

	//printf("========%s %d\n", dir_buf, S_ISDIR(astat.st_mode));
	//file_style(dir_buf);

	return S_ISDIR(astat.st_mode);
}

static void tree(DIR *dir)
{
	int i;
	struct dirent *item;
	int ret;
	int l_dir = last_dir;
	int current_no = no;
	
	DIR *newdir;

	while((item = readdir(dir)) != NULL){
		if(!strcmp(item->d_name, ".") || !strcmp(item->d_name, ".."))
			continue;

		if(is_dir(item->d_name)){
			last_dir = strlen(strbuf);

			strcat(strbuf, "/");
			strcat(strbuf, item->d_name);

			for(i = 0; i < current_no; i++)
				printf("%c", ' ');

			printf("|--%s\n", item->d_name);

			no+=3;

			newdir = opendir(strbuf);	
			if(newdir){
				tree(newdir);
				closedir(newdir);
			}else{
				strbuf[last_dir] = 0;//如果目录打开失败因该回到父目录
				printf("can not open dir %s\n", item->d_name);
			}
		}else{
			for(i = 0; i < current_no; i++)
				printf("%c", ' ');

			printf("|--%s\n", item->d_name);
		}
	}

	strbuf[l_dir] = 0;//回到父目录
	no-=3;
}

int main(int argc, char *argv[])
{
	int ret;
	DIR *dir;
	struct dirent *item;

	dir = opendir(argv[1]);
	if(!dir){
		perror("opendir");
		exit(1);
	}

	strcpy(strbuf, argv[1]);
	last_dir = 0;
	no = 0;
	

	printf("%s\n", argv[1]);

	tree(dir);

	closedir(dir);
	
	return 0;
}
