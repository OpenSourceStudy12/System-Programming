#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>

int pencil = 10;
pthread_mutex_t mutex;

void *thread_producer(void *argv)
{
	for (int i = 0; i < 100; i++)
	{
		pthread_mutex_lock(&mutex);
		++pencil;
		printf("thread produce a pencil %d\n", pencil);
		pthread_mutex_unlock(&mutex);
		usleep(500*1000);
	}
}

void *thread_consumer(void *argv)
{
	int id = (int)argv;

	for (;;)
	{
		pthread_mutex_lock(&mutex);
		if (pencil < 1)
		{
			pthread_mutex_unlock(&mutex);
			break;
		}

		printf("thread consumer[%d] sell a pencil [%d]\n", id, pencil);
		--pencil;
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}
}

int main()
{
	int ret;
	pthread_t pth_pro, pth_con1, pth_con2;

	pthread_mutex_init(&mutex, NULL);

	ret = pthread_create(&pth_pro, NULL, thread_producer, NULL);
	assert(ret == 0);
	ret = pthread_create(&pth_con1, NULL, thread_consumer, (void *)1);
	assert(ret == 0);
	ret = pthread_create(&pth_con2, NULL, thread_consumer, (void*)2);

	pthread_join(pth_pro, NULL);
	pthread_join(pth_con1, NULL);
	pthread_join(pth_con2, NULL);

	pthread_mutex_destroy(&mutex);
	return 0;
}
