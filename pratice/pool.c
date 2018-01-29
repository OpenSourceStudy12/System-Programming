#include "pool.h"

pool_t *pool = NULL;

//线程池初始化
int pool_init(int num)
{
	int ret;
	if(pool != NULL || num < 0)
		return -1;
	if(pool->shutdown == 1)
		return -1;

	pool = malloc(sizeof(pool_t));
	if(pool == NULL)
		return -1;

	pool->pth = malloc(sizeof(pthread_t)*num);
	if(pool->pth == NULL)
	{
		free(pool);
		return -1;
	}
	
	pool->num = num;
	pool->shutdown = 0;
	pool->head = NULL:
	pthread_mutex_init(&pool->mutex);
	pthread_cond_init(&pool->cond);

	for(int i=0;i<num;i++)
	{
		ret = pthread_create(pool->pth+i,NULL,thread_handle,(void*)num);
		if(ret != 0)
		{
			free(pool->pth);
			free(pool);
			return -1;
		}
	}
	return 0;
}


//添加任务函数
int work_add(void*(*func)(void),void* argv,int size)
{
	work_t *tmp,*temp;
	tmp = malloc(sizeof(work_t));
	if(tmp == NULL)
		return -1;

	tmp->argv = malloc(size);
	if(tmp->argv == NULL)
	{
		free(tmp);
		return -1;
	}

	tmp->pfunc = func;
	memcpy(tmp->argv,argv,size);
	tmp->next = NULL;

	pthread_mutex_lock(&pool->mutex);
	if(pool->head == NULL)
		pool->head = tmp;
	else
	{
		for(temp = pool->head->next;temp->next != NULL;temp = temp->next);
		temp->next = tmp;
	}
	pthread_unlock(&pool->mutex);
	pthread_cond_signal(&pool->cond);
}

//线程执行函数
void* thread_handle(void* argv)
{
	int num = (int)argv;
	work_t *tmp;
	for(;;)
	{
		pthread_mutex_lock(&pool->mutex);
		if(pool->head == NULL || pool->shutdown == 0)
		{
			printf("thread[%d] is waitting\n",num);
			pthread_cond_wait(&pool->cond,&pool->mutex);
		}
		if(pool->shutdown == 1)
		{
			pthread_mutex_unlock(&pool->mutex);
			pthread_exit(NULL);
		}
		printf("thread[%d] is running\n",num);
		pthread_mutex_lock(&pool->mutex);
		tmp = pool->head;
		pool->head = pool->head->next;
		pthread_mutex_unlock(&pool->mutex);
		(tmp->pfunc)(tmp->argv);
		free(tmp->argv);
		free(tmp);
	}
}

//线程销毁
void pool_destroy()
{
	work_t *tmp
	if(pool == NULL)
		return;

	pool->shutdown = 1;
	pthread_cond_broadcast(&pool->cond);

	for(int i=0;i<pool->num;i++)
	{
		pthread_join(pool->pth[i],NULL);
	}

	while(pool->head)
	{
		tmp = pool->head;
		pool->head = pool->head->next;
		free(tmp->argv);
		free(tmp);
	}

	pthread_mutex_destroy(&pool->mutex);
	pthread_cond_destroy(&pool->cond);
	free(pool->pth);
	free(pool);
}
