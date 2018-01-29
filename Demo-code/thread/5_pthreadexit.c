#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>

//exit:无论在进程的任何地方调用都可以退出整个进程
void fun()
{
	printf("in fun\n");
	pthread_exit(NULL);
}

void *thread_handle(void *argv)
{
//	printf("before exit\n");
//	exit(0);
//	pthread_exit(NULL);
	fun();
	for (int i = 0; i < 3; i++)
	{
		printf("new thread is running\n");
		usleep(500);
	}
//	printf("after exit\n");
}

int main()
{
	pthread_t pth;
	int ret;

	ret = pthread_create(&pth, NULL, thread_handle, NULL);
	assert(ret == 0);

	for (int i = 0; i < 5; i++)
	{
		printf("main thread is running\n");
		usleep(500);
	}
	return 0;
}
