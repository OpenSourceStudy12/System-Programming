#include "pool.h"

Pool pool;
pthread_mutex_t mysql_mutex; //数据库句柄互斥锁
pthread_mutex_t socketfd_mutex;//套接字句柄

//变量初始化
void var_init()
{
	pthread_mutex_init(&mysql_mutex,NULL);
	pthread_mutex_init(&socketfd_mutex,NULL);
}

/***********************************************************
 *function:线程池初始化，创建线程，初始化互斥锁、条件变量
 ***********************************************************/
int pool_init(int num)
{
	if(num <= 0)
		return -1;

	pool.shutdown = 0;
	pool.head = NULL;
	pool.pool_num = num;
	pthread_t* tmp = malloc(sizeof(pthread_t)*num);
	if(tmp == NULL)
		return -1;

	pool.pth = tmp;
	for(int i=0;i<num;i++)
	{
		pthread_create(tmp+i,NULL,thread_handle, (void*)i);
	}

	pthread_mutex_init(&pool.mutex,NULL);
	pthread_cond_init(&pool.cond,NULL);
	return 0;
}

/*******************************************************
 *function:线程处理
 *******************************************************/
void* thread_handle(void* argv)
{
	for(;;)
	{
		pthread_mutex_lock(&pool.mutex);//上锁
		while(pool.head == NULL && pool.shutdown == 0)
		{
			printf("thread[%d] is waitting\n",(int)argv);
			pthread_cond_wait(&pool.cond,&pool.mutex);//阻塞，解锁，有函数任务时解除阻塞，上锁
		}
		
		//线程结束
		if(pool.shutdown == 1)
		{
			pthread_mutex_unlock(&pool.mutex);
			pthread_exit(NULL);
		}
		assert(pool.head != NULL);

		printf("thread[%d] is running\n",(int)argv);
		Fun* tmp = pool.head;
		pool.head = pool.head->next;
		pthread_mutex_unlock(&pool.mutex);

//		pthread_mutex_lock(&socketfd_mutex);
		(tmp->func)(tmp->data);//执行真正的函数
//		pthread_mutex_unlock(&socketfd_mutex);
		free(tmp->data);//释放函数参数空间
		free(tmp);      //删除任务函数
	}
}

/*******************************************************
 *function:添加任务函数
 *******************************************************/
int func_add(void*(*func)(void*),void* argv,int size)
{
	if(func == NULL)
		return -1;
	Fun* tmp = malloc(sizeof(Fun)),*temp;
	if(tmp == NULL)
		return -1;

	//为函数参数申请空间
	tmp->data = malloc(sizeof(size));
	if(tmp->data == NULL)
	{
		free(tmp);
		return -1;
	}

	tmp->func = func;
	memcpy(tmp->data,argv,size);
	tmp->next = NULL;	

	pthread_mutex_lock(&pool.mutex);//上锁
	//添加任务函数到链表
	if(pool.head == NULL)
	{
		pool.head = tmp;
	}
	else
	{
		temp = pool.head;
		while(temp->next != NULL)
			temp = temp->next;
		temp->next = tmp;
	}

	pthread_mutex_unlock(&pool.mutex);//解锁
	pthread_cond_signal(&pool.cond);//解除一个线程的阻塞
}

/********************************************************
 *function:线程池的销毁
 ********************************************************/
void pool_destroy()
{
	Fun* tmp;
	pool.shutdown = 1;//线程结束标志置1

	//回收线程资源
	pthread_cond_broadcast(&pool.cond);//解除所有线程阻塞
	for(int i=0;i<pool.pool_num;i++)
	{
		pthread_join(pool.pth[i],NULL);
	}

	pthread_mutex_destroy(&pool.mutex);//销毁互斥锁
	pthread_cond_destroy(&pool.cond);//销毁条件变量

	free(pool.pth);//释放线程句柄空间

	while(pool.head != NULL)
	{
		tmp = pool.head;
		pool.head = pool.head->next;
		if(tmp->data != NULL)
			free(tmp->data);
		free(tmp);
	}
}
