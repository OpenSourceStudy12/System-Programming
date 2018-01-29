#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

//pthread_join:等待线程执行结束，回收线程资源，接受pthread_exit的返回值

void *thread_handle(void* argv)
{
	int num = 100;

	for (int i = 0; i < 5; i++)
	{
		printf("thread is running\n");
		sleep(1);
	}

	pthread_exit((void*)num);
}

int main()
{
	pthread_t pth;
	int ret;
	int num;
//	int *status = &num;
	int *status;

	ret = pthread_create(&pth, NULL, thread_handle, NULL);
	if (ret != 0)
	{
		fprintf(stderr, "create thread fail\n");
		return -1;
	}

	printf("before pthread_join\n");
	pthread_join(pth, (void*)&status);
	printf("after pthread_join: num = %d\n", (int)status);
	return 0;
}
