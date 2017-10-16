#ifndef __PROTOCOL_H
#define __PROTOCOL_H

#define NAME_SIZE	64

struct student_st {
	int age;
	int chiness;
	int english;
	char name[NAME_SIZE];
}__attribute__((packed));

#define SERVER_PORT 12345

#endif
