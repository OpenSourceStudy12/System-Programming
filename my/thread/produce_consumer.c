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
int flag = 0;
pthread_mutex_t mutex;
pthread_cond_t cond;

void link_create(PPen head,Pen pen)
{
	PPen tmp = malloc(sizeof(Pen));
	*tmp = pen;
	tmp->next = head;
	head = tmp;
}
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
void* produce_handle(void* argv)
{
	for(int i=0;i<12;i++)
	{
		Pen pen;
		strcpy(pen.pro,"lrh");
		pen.price = 2.0;
		pthread_mutex_lock(&mutex);
		link_create(head,pen);
		printf("in produce..\n");
		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&cond);
		usleep(500*1000);
	}
}

void* consumer_handle(void* argv)
{
	while(1)
	{	
		pthread_mutex_lock(&mutex);
		if(head == NULL /*&& flag == 0*/)
		{
			printf("in consumer,in waitting..\n");
			pthread_cond_wait(&cond,&mutex);
		}
		if(flag == 1)
		{
			pthread_mutex_unlock(&mutex);
			return NULL;
		}

		link_delete(head);
		printf("delete pencil..\n");
		pthread_mutex_unlock(&mutex);
		usleep(500*1000);
	}
}


int main()
{
	int ret;
	pthread_t pth_pro,pth_con;

	ret = pthread_create(&pth_pro,NULL,produce_handle,NULL);
	assert(ret == 0);
	ret = pthread_create(&pth_con,NULL,consumer_handle,NULL);
	assert(ret == 0);

	pthread_mutex_init(&mutex,NULL);
	pthread_cond_init(&cond,NULL);

	pthread_join(pth_pro,NULL);
	flag = 1;
	pthread_cond_broadcast(&cond);
	pthread_join(pth_con,NULL);


	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);

	link_free(head);

	return 0;
}
