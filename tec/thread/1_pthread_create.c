#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

//线程是程序执行的一条路径
//主函数结束，线程执行结束。 主线程执行结束， 进程才会结束。 进程结束，所有线程都会结束
//进程的任何地方调用exit都会结束整个进程
void *thread_handle(void *argv)
{
	for (int i = 0; i < 5; i++)
	{
		printf("in thread_handle the %d times\n", i);
		sleep(1);
	}

	exit(0);
}

int main()
{
	pthread_t pth;
	int ret;

	ret = pthread_create(&pth, NULL, thread_handle, NULL);
	if (ret != 0)
	{
		fprintf(stderr, "create thread fail \n");
		return -1;
	}

	for (int i = 0; i < 10; i++)
	{
		printf("in main thread, the %d times\n", i);
		sleep(1);
	}
	return 0;
}
