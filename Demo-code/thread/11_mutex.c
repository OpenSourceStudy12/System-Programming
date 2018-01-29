#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>

int tickets = 100;
//pthread_mutex_t mutex;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void *thread_handle1(void *argv)
{
	for (;;)
	{
		pthread_mutex_lock(&mutex);
		if (tickets > 0)
		{
			usleep(500);
			printf("thread[%lu] sale tickets %d\n", pthread_self(), tickets--);
		}
		else
		{
			pthread_mutex_unlock(&mutex);
			break;
		}
		pthread_mutex_unlock(&mutex);
	}

	return NULL;
}

void *thread_handle2(void *argv)
{
	for (;;)
	{
		pthread_mutex_lock(&mutex);
		if (tickets > 0)
		{
			usleep(500);
			printf("thread[%lu] sale tickets %d\n", pthread_self(), tickets--);
		}
		else
		{
			pthread_mutex_unlock(&mutex);
			break;
		}
		pthread_mutex_unlock(&mutex);
	}

	return NULL;
}

int main()
{
	pthread_t pth1, pth2;
	int ret;
//	pthread_mutex_init(&mutex, NULL);

	ret = pthread_create(&pth1, NULL, thread_handle1, NULL);
	assert(ret == 0);
	ret = pthread_create(&pth2, NULL, thread_handle2, NULL);
	assert(ret == 0);

	pthread_join(pth1, NULL);
	pthread_join(pth2, NULL);
	return 0;
}
