#include"pool.h"

PPool  pool = NULL;
Cli* head = NULL;

void list_create(int clientfd)
{
	Cli* tmp = malloc(sizeof(clientfd));
	tmp->clientfd = clientfd;
	tmp->next = head;
	head = tmp;
}

void node_del(int clientfd)
{
	Cli *tmp = head,*temp;
	if(head->clientfd == clientfd)
	{
		head = head->next;
		free(tmp);
		return;
	}
	while(tmp->next != NULL)
	{
		temp = tmp->next;
		if(temp->clientfd == clientfd)
		{
			tmp->next = temp->next;
			free(temp);
			return;
		}
		tmp = tmp->next;
	}
}


void pool_destroy()
{
	if(pool == NULL)
		return;

	pool->shutdown = 1;
	pthread_cond_broadcast(&pool->cond);

	for(int i=0;i<pool->num;i++)
	{
		pthread_join(pool->pth[i],NULL);
	}

	pthread_mutex_destroy(&pool->mutex);
	pthread_cond_destroy(&pool->cond);

	PFunc tmp = NULL;
	while(pool->head!= NULL)
	{
		tmp = pool->head;
		pool->head = pool->head->next;
		free(tmp->argv);
		free(tmp);
	}

	free(pool->pth);	
	free(pool);
}

int func_add(void* (*pfunc)(void*),void* argv,int size)
{
	if(pool == NULL)
		return -1;
	if(pool->shutdown == 1)
		return -1;
	
	PFunc tmp = malloc(sizeof(Func));
	if(tmp == NULL)
		return -1;
	tmp->pfunc = pfunc;
	tmp->argv = malloc(size);
	printf("argv = %p\n",tmp->argv);
	memcpy(tmp->argv,argv,size);
	printf("argv = %p\n",tmp->argv);
	pthread_mutex_lock(&pool->mutex);
	tmp->next = pool->head;
	pool->head = tmp;
	pthread_mutex_unlock(&pool->mutex);
	pthread_cond_signal(&pool->cond);
}

void* thread_handle(void* argv)
{
	PFunc tmp = NULL;
	while(1)
	{
		pthread_mutex_lock(&pool->mutex);
		if(pool->head == NULL && pool->shutdown == 0)
		{
			printf("thread[%lu] is waitting\n",pthread_self());
			pthread_cond_wait(&pool->cond,&pool->mutex);
		}
		if(pool->shutdown == 1)
		{
			pthread_mutex_unlock(&pool->mutex);
			return NULL;
		}

		tmp = pool->head;
		pool->head = pool->head->next;
		printf("thread[%lu] is running\n",pthread_self());
		pthread_mutex_unlock(&pool->mutex);

		(tmp->pfunc)(tmp->argv);
		
		free(tmp->argv);
		free(tmp);
	}
}

int pool_init(int num)
{
	if(pool != NULL)
		return -1;
	if(num < 1)
		return -1;

	pool = malloc(sizeof(Pool));
//	assert(pool != NULL);
	if(pool == NULL)
		return -1;

	pool->pth = malloc(sizeof(pthread_t)*num);
	if(pool->pth == NULL)
	{
		free(pool);
		return -1;
	}

	pool->head = NULL;
	pool->num = num;
	pool->shutdown = 0;

	pthread_mutex_init(&pool->mutex,NULL);
	pthread_cond_init(&pool->cond,NULL);
	for(int i=0;i<num;i++)
	{
		if(pthread_create(pool->pth+i,NULL,thread_handle,NULL))
		{
			free(pool->pth);
			free(pool);
			pool = NULL;
			return -1;
		}
	}
	return 0;
}
