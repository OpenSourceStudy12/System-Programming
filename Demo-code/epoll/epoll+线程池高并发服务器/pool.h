#ifndef __POOL_H
#define __POOL_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>

typedef struct worker
{
	void *(*pfun)(void *);	//真正需要执行的函数的函数指针
	void *argv;				//真正执行函数的入参
	struct worker *next;
}WORKER;

typedef struct
{
	int number;		//线程数量
	pthread_t *pth;	//线程标识符
	int shutdown;	//线程池销毁标志
	WORKER *head;	//线程实现函数链表的头指针
	pthread_mutex_t mutex;
	pthread_cond_t cond;
}pool_t;

int pool_init(int number);
int pool_add_worker(void *(*pfun)(void*), void *argv, size_t size);
int pool_destroy();

#endif
