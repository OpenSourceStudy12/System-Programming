#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <assert.h>

void *thread_handle(void *argv)
{
	char name[10];
	int ret;

	for (int i = 0; i < 10; i++)
	{
//		memset(name, 0, sizeof(name));
		bzero(name, sizeof(name));
		sprintf(name, "file%d", i);
		ret = creat(name, 0640);
		if (ret < 0)
			pthread_exit((void *)1);
	}

	pthread_exit((void *)0);
}

int main()
{
	pthread_t pth;
	int ret;
	char name[10];
	int *status;

	ret = pthread_create(&pth, NULL, thread_handle, NULL);
	assert(ret == 0);

	pthread_join(pth, (void **)&status);
	if ((int)status == 1)
	{
		fprintf(stderr, "创建文件失败\n");
		return -1;
	}
	else if ((int)status == 0)
	{
		printf("创建文件成功\n");
		sleep(10);
		for (int i = 0; i < 10; i++)
		{
			memset(name, 0, sizeof(name));
			sprintf(name, "file%d", i);
			remove(name);
		}
	}

	return 0;
}
