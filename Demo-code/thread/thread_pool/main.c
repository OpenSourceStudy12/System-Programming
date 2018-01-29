#include "pool.h"

void *thread_handle(void *argv)
{
	int num = *(int*)argv;
	for (int i = 0; i < 5; i++)
	{
		printf("thread[%lu] num = %d\n", pthread_self(), num);
		sleep(1);
	}

	return NULL;
}

int main()
{
	int ret;

	ret = pool_init(5);
	if (ret < 0)
	{
		printf("初始化线程池失败\n");
		return -1;
	}

	int num = 10;
	for (int i = 0; i < 4; i ++)
	{
		ret = pool_add_worker(thread_handle, &num, sizeof(num));
		if (ret < 0)
		{
			printf("添加执行函数失败\n");
			return -1;
		}
	}
	for (int i = 0; i < 10; i++)
		sleep(1);

	ret = pool_destroy();
	if (ret < 0)
	{
		printf("销毁线程池失败\n");
		return -1;
	}

	return 0;
}
