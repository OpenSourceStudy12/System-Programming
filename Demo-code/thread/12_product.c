#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>

int produce = 0;
pthread_mutex_t mutex;

void *thread_pro(void *argv)
{
	for (;;)
	{
		pthread_mutex_lock(&mutex);
		printf("thead_pro product [%d]\n", produce++);
		usleep(500*1000);

		pthread_mutex_unlock(&mutex);
	}
}

void *thread_cos(void *argv)
{
	int id = (int)argv;
	for (;;)
	{
		pthread_mutex_lock(&mutex);
		if (produce > 0)
		{
			printf("thread[%d] using product[%d]\n", id, produce--);
			usleep(500*1000);
		}
		else
		{
			pthread_mutex_unlock(&mutex);
			continue;
		}
		pthread_mutex_unlock(&mutex);
	}
}

int main()
{
	pthread_t cos1, cos2, pro;
	int ret;
	pthread_mutex_init(&mutex, NULL);

	ret = pthread_create(&pro, NULL, thread_pro, NULL);
	assert(ret == 0);
	ret = pthread_create(&cos1, NULL, thread_cos, (void*)1);
	assert(ret == 0);
	ret = pthread_create(&cos2, NULL, thread_cos, (void*)2);
	assert(ret == 0);

	pthread_join(pro, NULL);
	pthread_join(cos1, NULL);
	pthread_join(cos2, NULL);

	return 0;
}
