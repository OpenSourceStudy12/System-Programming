#ifndef _POOL_H
#define _POOL_H

#include "commen.h"
#include <pthread.h>

typedef struct fun
{
	void*(*func)(void*); //函数指针
	void* data;          //函数入参
	struct fun* next;
}Fun;

typedef struct pool
 {
	int pool_num;            //线程数目
	pthread_t* pth;          //线程句柄
	pthread_mutex_t mutex;   //互斥锁变量
	pthread_cond_t cond;     //条件变量
	int shutdown;            //线程结束标志
	Fun* head;               //任务函数头指针
}Pool;

extern pthread_mutex_t mysql_mutex;
extern pthread_mutex_t socketfd_mutex;


int pool_init(int num);
void* thread_handle(void* argv);
int func_add(void*(*func)(void*),void* argv,int size);
void pool_destroy();

#endif
