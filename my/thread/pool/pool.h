#ifndef _POOL_H
#define _POOL_H

#include<stdio.h>
#include<pthread.h>
#include<assert.h>
#include<unistd.h>
#include<stdlib.h>

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
int func_add(void* (*pfunc)(void*),void* argv);
int pool_init(int num);

#endif

