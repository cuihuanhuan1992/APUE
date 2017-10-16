#ifndef __PROTOCOL_H
#define __PROTOCOL_H

#define NAME_SIZE	1024

#define REQUEST_R	0
#define REQUEST_W	1

struct request_st {
	char request;
	char options;
	char content[NAME_SIZE];
	
}__attribute__((packed));

#define RESPOND_ERROR	0
#define RESPOND_OK	1

struct respond_st {
	char respond;
	char options;
	int len;
	char content[NAME_SIZE];
}__attribute__((packed));

#define SERVER_PORT 12345

#endif
