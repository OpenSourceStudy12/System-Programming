#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>

int tickets = 100;

void *thread_handle1(void *argv)
{
	for (;;)
	{
		if (tickets >= 0)
		{
			usleep(500);
			printf("thread[%lu] sale tickets %d\n", pthread_self(), tickets--);
		}
		else
			break;
	}

	return NULL;
}

void *thread_handle2(void *argv)
{
	for (;;)
	{
		if (tickets >= 0)
		{
			usleep(500);
			printf("thread[%lu] sale tickets %d\n", pthread_self(), tickets--);
		}
		else
			break;
	}

	return NULL;
}

int main()
{
	pthread_t pth1, pth2;
	int ret;

	ret = pthread_create(&pth1, NULL, thread_handle1, NULL);
	assert(ret == 0);
	ret = pthread_create(&pth2, NULL, thread_handle2, NULL);
	assert(ret == 0);

	pthread_join(pth1, NULL);
	pthread_join(pth2, NULL);
	return 0;
}
