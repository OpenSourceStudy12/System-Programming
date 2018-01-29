#ifndef _POOL_H
#define _POOL_H

#include <stdio.h>
#include <pthread.h>
#include <asset.h>
#include <unistd.h>

typedef struct work
{
	void* (*pfunc)(void*);
	void* argv;
	work *next;
}work_t;

typedef struct
{
	int num;
	pthread_t *pth;
	int shutdown;
	work_t *head;
	pthread_cond_t cond;
	pthread_mutex_t mutex;
}pool_t;

int pool_init(int num);
int work_add(void*(*func)(void),void* argv,int size);
void* thread_handle(void* argv);
void pool_destroy();
#endif
