#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>

pthread_mutex_t mutex;
pthread_cond_t cond;
int shutdown = 0;
int flag = 0;

struct people
{
//	int flag;
	char name[10];
	char sex;
	struct people *next;
};

struct people *head = NULL;

void *thread_pro(void *argv)
{
	int i = 0;
	for (;;)
	{
		struct people hum;
		if (i%2 == 0)
		{
//			hum.flag = 1;
			strcpy(hum.name, "zhangsan");
	//		usleep(500);
			hum.sex = 'M';
		}
		else if (i%2 == 1)
		{
//			hum.flag = 1;
			strcpy(hum.name, "lisi");
	//		usleep(500);
			hum.sex = 'F';
		}
		i++;

		struct people *temp = malloc(sizeof(hum));
		assert(temp != NULL);

		temp->sex = hum.sex;
		strcpy(temp->name, hum.name);
		temp->next = NULL;

		pthread_mutex_lock(&mutex);
		if (shutdown == 1)
		{
			pthread_mutex_unlock(&mutex);
			return NULL;
		}
		
		if (head == NULL)
			head = temp;
		else
		{
			temp->next = head;
			head = temp;
		}
		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&cond);
	}
}

void *thread_cos(void *argv)
{
	int id = (int)argv;
	for (;;)
	{
		pthread_mutex_lock(&mutex);
		
/*		if (hum.flag == 0)
			pthread_cond_wait(&cond, &mutex);
			*/
		while (head == NULL && shutdown == 0)
			pthread_cond_wait(&cond, &mutex);
		if (shutdown == 1)
		{
			pthread_mutex_unlock(&mutex);
			return NULL;
		}

		struct people hum = *head;
		printf("thread[%lu] name = %s, sex = %c\n", pthread_self(), hum.name, hum.sex);
		struct people *temp = head;
		head = head->next;
		free(temp);
//		hum.flag = 0;
//		usleep(500);
		
		pthread_mutex_unlock(&mutex);
	}
}
int main()
{
	pthread_t pro, cos1, cos2;
	int ret;

	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);

	ret = pthread_create(&pro, NULL, thread_pro, NULL);
	assert(ret == 0);
	ret = pthread_create(&cos1, NULL, thread_cos, (void *)1);
	assert(ret == 0);
	ret = pthread_create(&cos2, NULL, thread_cos, (void *)2);
	assert(ret == 0);

	for (int i = 0; i < 100; i++)
		usleep(50);
	shutdown = 1;
	pthread_join(pro, NULL);
	pthread_cond_broadcast(&cond);
	pthread_join(cos1, NULL);
	pthread_join(cos2, NULL);

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);

	return 0;
}
