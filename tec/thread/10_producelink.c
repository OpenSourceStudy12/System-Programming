#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>

struct produce
{
	int id;
	char name[20];
};

struct node
{
	struct produce pro;
	struct node *next;
};

struct node *head = NULL;
pthread_cond_t cond;
pthread_mutex_t mutex;

int flag = 0;

void *thread_producer(void *argv)
{
	struct produce pro;
	struct node *temp, *current;

	for (int i = 0; i < 100; ++i)
	{
		if (i%3 == 0)
		{
			pro.id = i+1000;
			strcpy(pro.name, "apple");
		}
		else if (i%3 == 1)
		{
			pro.id = i+1000;
			strcpy(pro.name, "huawei");
		}
		else
		{
			pro.id = i+1000;
			strcpy(pro.name, "oppo");
		}

		current = malloc(sizeof(struct node));
		current->pro = pro;
		current->next = NULL;

		pthread_mutex_lock(&mutex);
		if (head == NULL)
		{
			head = current;
		}
		else 
		{
			temp = head;
			while (temp->next != NULL)
				temp = temp->next;
			temp->next = current;
		}
		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&cond);
		usleep(50*1000);
	}
}

void *thread_consumer(void *argv)
{
	int id = (int )argv;
	struct node *current;

	for (;;)
	{
		pthread_mutex_lock(&mutex);
		if (head == NULL && flag == 0)
		{
			pthread_cond_wait(&cond, &mutex);
		}

		if (flag  == 1)
		{
			pthread_mutex_unlock(&mutex);
			return NULL;
		}

		printf("thread[%d] use produce: id = %d, name = %s\n", id, head->pro.id, head->pro.name);
		current = head;
		head = head->next;
		pthread_mutex_unlock(&mutex);
		free(current);
	}
}

int main()
{
	pthread_t pth[3];
	int ret;

	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);

	ret = pthread_create(&pth[0], NULL, thread_producer, NULL);
	assert(ret == 0);

	for (int i = 1; i < 3; i++)
	{
		ret = pthread_create(&pth[i], NULL, thread_consumer, (void*)i);
		assert(ret == 0);
	}

	pthread_join(pth[0], NULL);
	flag = 1;
	pthread_cond_broadcast(&cond);

	pthread_join(pth[1], NULL);
	pthread_join(pth[2], NULL);

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
	return 0;
}
