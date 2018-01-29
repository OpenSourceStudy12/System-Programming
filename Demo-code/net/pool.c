#include "pool.h"

pool_t *pool = NULL;

int pool_destroy()
{
	if (pool == NULL)
		return -1;
	if (pool->shutdown == 1)
		return -1;

	pool->shutdown = 1;	//此时线程函数可以结束
	pthread_cond_broadcast(&pool->cond);	//发送广播信号，解除所有线程函数的阻塞

	for (int i = 0; i < pool->number; i++)
	{
		pthread_join((pool->pth)[i], NULL);	//等待所有的线程函数结束
	}

	WORKER *temp = pool->head;
	//销毁还没来得及执行的所有执行函数链表
	while (pool->head != NULL)
	{
		temp = pool->head;
		pool->head = pool->head->next;		
		free(temp->argv);
		free(temp);
	}

	free(pool->pth);
	free(pool);
	pool = NULL;
}

//线程池创建的线程函数
void *pool_routine(void *argv)
{
	WORKER *current;
	for (;;)
	{
		pthread_mutex_lock(&(pool->mutex));
		while (pool->head == NULL && pool->shutdown == 0)
		{
			//当没有可执行函数时，阻塞
			printf("thread[%lu] is waitting\n", pthread_self());
			pthread_cond_wait(&pool->cond, &pool->mutex);
		}
		if (pool->shutdown == 1)
		{
			pthread_mutex_unlock(&pool->mutex);
			pthread_exit(NULL);
		}
	
		assert(pool->head != NULL);
		current = pool->head;
		
		pool->head = pool->head->next;
		pthread_mutex_unlock(&pool->mutex);
		
		printf("thread[%lu] is runnig\n", pthread_self());
		//通过函数指针调用真正的执行函数
		(current->pfun)(current->argv);

		free(current->argv);
		free(current);
	}
}

//添加真正的执行函数
int pool_add_worker(void*(*pfun)(void *), void *argv, size_t size)
{
	WORKER *temp, *tail;

	temp = malloc(sizeof(WORKER));
	if (temp == NULL)
		return -1;

	//将真正的执行函数赋值给函数指针
	temp->pfun = pfun;
	temp->argv = malloc(size);
	if (temp->argv == NULL)
	{
		free(temp);
		return -1;;
	}
	//将执行函数的入参拷贝到结构体中
	memcpy(temp->argv, argv, size);
	temp->next = NULL;

	pthread_mutex_lock(&pool->mutex);
	if (pool->head == NULL)
		pool->head = temp;
	else
	{
		tail = pool->head;
		while (tail->next)
			tail = tail->next;
		tail->next = temp;
	}
	pthread_mutex_unlock(&pool->mutex);
	//发送信号告诉线程函数，现在有可执行函数
	pthread_cond_signal(&pool->cond);
}

int pool_init(int number)
{
	if (pool != NULL)
		return -1;

	if (number <= 0)
		return -1;

	pool = malloc(sizeof(pool_t));
	if (pool == NULL)
		return -1;

	//初始化pool变量
	pool->number = number;
	pool->shutdown = 0;		//shutdown等于1时， 销毁线程池
	pool->pth = malloc(sizeof(pthread_t)*number);
	if (pool->pth == NULL)
	{
		free(pool);
		return -1;
	}
	pthread_mutex_init(&pool->mutex, NULL);
	pthread_cond_init(&pool->cond, NULL);
	pool->head = NULL;

	for (int i = 0; i < number; i++)
	{
		//创建number个线程
		if (pthread_create((pool->pth)+i, NULL, pool_routine, NULL) != 0)
		{
			free(pool->pth);
			free(pool);
			return -1;
		}
	}

	return 0;
}
