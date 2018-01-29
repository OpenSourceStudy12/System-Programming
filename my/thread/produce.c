#include<stdio.h>
#include<assert.h>
#include<pthread.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

typedef struct Pencil
{
	char pro[20];
	double price;
	//int num;
	struct Pencil* next;
}Pen,*PPen;

PPen head = NULL;
int flag = 0;//产商线程结束标志
pthread_mutex_t mutex;//互斥锁变量
pthread_cond_t cond;//条件变量

//头插法创建链表
void link_create(PPen head,Pen pen)
{
	PPen tmp = malloc(sizeof(Pen));
	*tmp = pen;
	tmp->next = head;
	head = tmp;
}
//删除节点
void link_delete(PPen head)
{
	if(head != NULL)
	{
		PPen tmp = head;
		head = head->next;
		free(tmp);
		tmp = NULL;
	}
}
//释放链表
void link_free(PPen head)
{
	if(head != NULL)
	{
		PPen tmp;
		while(head != NULL)
		{
			tmp = head;
			head = head->next;
			free(tmp);
		}
	}
}
//产商线程
void* produce_handle(void* argv)
{
	for(int i=0;i<12;i++)
	{
		Pen pen;
		strcpy(pen.pro,"lrh");
		pen.price = 2.0;
		pthread_mutex_lock(&mutex);//上锁
		link_create(head,pen);
		printf("in produce..\n");
		pthread_mutex_unlock(&mutex);//解锁
		pthread_cond_signal(&cond);//释放一个线程阻塞
		usleep(500*1000);
	}
}
//消费者线程
void* consumer_handle(void* argv)
{
	while(1)
	{	
		pthread_mutex_lock(&mutex);//上锁
		if(head == NULL /*&& flag == 0*/)
		{
			printf("in consumer,in waitting..\n");
			pthread_cond_wait(&cond,&mutex);//消费者线程进入阻塞，并解锁。当收到信号时，解除阻塞，并上锁
		}
		if(flag == 1)
		{
			pthread_mutex_unlock(&mutex);//解锁
			return NULL;
		}

		link_delete(head);
		printf("delete pencil..\n");
		pthread_mutex_unlock(&mutex);//解锁
		usleep(500*1000);
	}
}


int main()
{
	int ret;
	pthread_t pth_pro,pth_con;

	ret = pthread_create(&pth_pro,NULL,produce_handle,NULL);//创建产商线程
	assert(ret == 0);
	ret = pthread_create(&pth_con,NULL,consumer_handle,NULL);//创建消费者线程
	assert(ret == 0);

	pthread_mutex_init(&mutex,NULL);//初始化互斥锁
	pthread_cond_init(&cond,NULL);//初始化条件变量

	pthread_join(pth_pro,NULL);
	flag = 1;
	pthread_cond_broadcast(&cond);//解除所有线程阻塞
	pthread_join(pth_con,NULL);


	pthread_mutex_destroy(&mutex);//销毁互斥锁
	pthread_cond_destroy(&cond);//销毁条件变量

	link_free(head);//释放链表

	return 0;
}
