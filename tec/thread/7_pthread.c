#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>

int tickets = 100;
pthread_mutex_t mutex;

void *thread_handle(void *argv)
{
	int id = (int)argv;
	for (;;)
	{
		pthread_mutex_lock(&mutex);
		if (tickets > 0)
		{
		//	usleep(100);
			printf("thread[%d] sold tickets %d\n", id, tickets--);
		}
		else 
		{
			pthread_mutex_unlock(&mutex);
			break;
		}
		pthread_mutex_unlock(&mutex);
		usleep(100);
	}
}

int main()
{
	int ret;
	pthread_t pth1, pth2;

	pthread_mutex_init(&mutex, NULL);

	ret = pthread_create(&pth1, NULL, thread_handle, (void*)1);
	assert(ret == 0);
	ret = pthread_create(&pth2, NULL, thread_handle, (void*)2);
	assert(ret == 0);

	pthread_join(pth1, NULL);
	pthread_join(pth2, NULL);

	pthread_mutex_destroy(&mutex);
	return 0;
}
