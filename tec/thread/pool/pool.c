#include "pool.h"

pool_t *pool = NULL;
//销毁线程池
int pool_destroy()
{
	if (pool == NULL)
		return -1;
	if (pool->shutdown == 1)
		return -1;

	pool->shutdown = 1;
	pthread_cond_broadcast(&pool->cond);//解除所有线程的阻塞
	//回收所有线程的资源 阻塞
	for (int i = 0; i < pool->number; i++)
	{
		pthread_join((pool->pth)[i], NULL);
	}
	//删除所有任务函数
	worker_t *temp;
	while (pool->head != NULL)
	{
		temp = pool->head;
		pool->head = pool->head->next;
		free(temp->argv);//释放参数空间
		free(temp);//释放任务
	}

	pthread_mutex_destroy(&pool->mutex);//销毁互斥锁
	pthread_cond_destroy(&pool->cond);//销毁条件变量

	free(pool->pth);//释放线程号空间
	free(pool);//释放线程池
	pool = NULL;
	return 0;
}
//添加执行函数
int pool_add_worker(void *(*pfunc)(void*), void *argv, size_t size)
{
	if (pool == NULL)
		return -1;
	if (pool->shutdown == 1)
		return -1;
	
	worker_t *current, *temp;

	current = malloc(sizeof(worker_t));//申请任务空间
	assert(current != NULL);

	current->pfunc = pfunc;//传入函数指针
	current->argv = malloc(size);//函数参数指针
	assert(current->argv != NULL);
	current->argv = argv;
	//拷贝函数参数
	memcpy(current->argv, argv, size);
	current->next = NULL;

	pthread_mutex_lock(&pool->mutex);//上锁
	//添加任务函数到链表中
	if (pool->head == NULL)
		pool->head = current;
	else 
	{
		temp = pool->head;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = current;
	}
	pthread_mutex_unlock(&pool->mutex);//解锁
	pthread_cond_signal(&pool->cond);//解除某个线程的阻塞
}
//线程执行函数
void *thread_routine(void *argv)
{
	worker_t *temp;
	for (;;)
	{
		pthread_mutex_lock(&pool->mutex);//上锁
		while (pool->head == NULL && pool->shutdown == 0)
		{
			printf("thread[%lu] is waitting\n", pthread_self());
			pthread_cond_wait(&pool->cond, &pool->mutex);//线程阻塞，解锁，解除阻塞后上锁
		}

		if (pool->shutdown == 1)//线程结束标志
		{
			pthread_mutex_unlock(&pool->mutex);//解锁
			pthread_exit(NULL);
		}
		assert(pool->head != NULL);

		temp = pool->head;
		pool->head = pool->head->next;
		pthread_mutex_unlock(&pool->mutex);//解锁
		
		printf("thread[%lu] is running\n", pthread_self());//pthread_self()获取线程号
		(temp->pfunc)(temp->argv);//回调函数
		free(temp->argv);
		free(temp);
	}
}
//线程初始化，创建线程的数量
int pool_init(int number)
{
	if (pool != NULL)
		return -1;
	if (number < 1)
		return -1;

	pool = malloc(sizeof(pool_t));//申请线程池空间
	assert(pool != NULL);

	pool->head = NULL;//任务函数指针置空
	pool->pth = malloc(sizeof(pthread_t)*number);//线程号空间
	if (pool->pth == NULL)
	{
		free(pool);
		return -1;
	}
	//赋值
	pool->number = number;
	pool->shutdown = 0;
	pthread_mutex_init(&pool->mutex, NULL);//初始化互斥锁
	pthread_cond_init(&pool->cond, NULL);//初始化条件变量
	//创建线程
	for (int i = 0; i < number; i++)
	{
		if (pthread_create(pool->pth+i, NULL, thread_routine, NULL) != 0)
		{
			free(pool->pth);
			free(pool);
			return -1;
		}
	}

	return 0;
}
