#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

void *thread_handle(void *argv)
{
	int num = 255;
	for (int i = 0; i < 5; i++)
	{
//		return NULL;
		printf("thread[%lu] is running\n", pthread_self());
		sleep(1);
	}

//	return NULL;
	printf("%p\n", (void *)num);
	pthread_exit((void *)num);
}

int main()
{
	pthread_t pth;
	int ret;
//	int *status;
	char *status;

	ret = pthread_create(&pth, NULL, thread_handle, NULL);
	assert(ret == 0);

	printf("before pthread_join\n");
	pthread_join(pth, (void**)&status);
	printf("after pthread_join\n");
	printf("%p\n", status);
	printf("status = %d\n", (int)status);
	return 0;
}
