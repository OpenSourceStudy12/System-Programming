#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>

int global = 100;

void *thread_handle(void *argv)
{
	printf("global = %d\n", global);
	global = 200;
}

int main()
{
	pthread_t pth;
	int ret;

	ret = pthread_create(&pth, NULL, thread_handle, NULL);
	assert(ret == 0);

	pthread_join(pth, NULL);
	printf("global = %d\n", global);
	return 0;
}
