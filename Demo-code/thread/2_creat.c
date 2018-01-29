#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void *thread_handle(void *argv)
{
	for (int i = 0; i < 5; i++)
	{
		printf("new thread[%lu] is running\n", pthread_self());
		usleep(500);
	}
}

int main()
{
	pthread_t pth[5];

	for (int i = 0; i < 5; i++)
	{
		if (pthread_create(&pth[i], NULL, thread_handle, NULL) != 0)
		{
			fprintf(stderr, "创建线程失败\n");
			return -1;
		}
	}
	/*
	   if (pthread_create(&pth2, NULL, thread_handle, NULL) != 0)
	   {
	   fprintf(stderr, "创建线程失败\n");
	   return -1;
	   }
	 */
	for (int i = 0; i < 10; i++)
	{
		printf("main thread is running\n");
		usleep(500);
	}

	return 0;
}
