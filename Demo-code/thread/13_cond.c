#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>

int produce = 0;
pthread_mutex_t mutex;
pthread_cond_t cond;

void *thread_pro(void *argv)
{
	for (;;)
	{
		pthread_mutex_lock(&mutex);
		printf("thead_pro product [%d]\n", produce++);

		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&cond);
		usleep(500*1000);
	}
}

void *thread_cos(void *argv)
{
	int id = (int)argv;
	for (;;)
	{
		pthread_mutex_lock(&mutex);
		while (produce <= 0)
			pthread_cond_wait(&cond, &mutex);

		printf("thread[%d] using product[%d]\n", id, produce--);
		pthread_mutex_unlock(&mutex);
		usleep(500*1000);
	}
}

int main()
{
	pthread_t cos1, cos2, pro;
	int ret;
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);

	ret = pthread_create(&pro, NULL, thread_pro, NULL);
	assert(ret == 0);
	ret = pthread_create(&cos1, NULL, thread_cos, (void*)1);
	assert(ret == 0);
	ret = pthread_create(&cos2, NULL, thread_cos, (void*)2);
	assert(ret == 0);

	pthread_join(pro, NULL);
	pthread_join(cos1, NULL);
	pthread_join(cos2, NULL);
	pthread_cond_destroy(&cond);

	return 0;
}
