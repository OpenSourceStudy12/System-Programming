#ifndef _POOL_H
#define _POOL_H

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>
#include<string.h>
#include<assert.h>
#include<sys/wait.h>
#include<pthread.h>

typedef struct client
{
	int clientfd;
//	struct sockaddr_in cli_add;
}Cli;

typedef struct func
{
	void* (*pfunc)(void*);
	void* argv;
	struct func* next;
}Func,*PFunc;

typedef struct pool
{
	int num;
	pthread_t *pth;
	PFunc head;
	int shutdown;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
}Pool,*PPool;


void pool_destroy();
int func_add(void* (*pfunc)(void*),void* argv,int size);
int pool_init(int num);

#endif

