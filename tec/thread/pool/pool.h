#ifndef __POOL_H
#define __POOL_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <pthread.h>

typedef struct worker
{
	void *(*pfunc)(void*);//回调函数
	void *argv;//函数参数
	struct worker *next;
}worker_t;

typedef struct 
{
	int number;//线程池中线程的个数
	int shutdown;//线程池的销毁标志
	worker_t *head;//(产品)函数的链表头指针
	pthread_t *pth;//线程号指针
	pthread_mutex_t mutex;
	pthread_cond_t cond;
}pool_t;

int pool_destroy();
int pool_add_worker(void *(*pfunc)(void*), void *argv, size_t size);
int pool_init(int number);
#endif
