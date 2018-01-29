#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *thread_handle(void *argv)
{
//	for (int i = 0; i < 50; i++)
	for (;;)
	{
		printf("new thread is running\n");
//		usleep(500*1000);
	}

	return NULL;
}

int main()
{
	pthread_t pth;
	int ret;		//

	ret = pthread_create(&pth, NULL, thread_handle, NULL);
	if (ret != 0)
	{
		fprintf(stderr, "创建线程失败\n");
		return -1;
	}

//	for (int i = 0; i < 100; i++)
	for (;;)
	{
		printf("main thread is running\n");
//		usleep(500*1000);
	}

	return 0;
}
