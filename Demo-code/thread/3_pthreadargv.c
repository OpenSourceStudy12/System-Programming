#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

void *thread_handle(void *argv)
{
//	int *num = (int *)argv;
//	int num = *(int *)argv;
//	printf("num = %d\n", num);
//	num = 200;

	char *Tname = (char *)argv;
	printf("in thread name = %s\n", Tname);
	strcpy(Tname, "lisi");
	return NULL;
}

int main()
{
	pthread_t pth;
	int num, ret;

	num = 10;
//	char *name = "zhangsan";
	char name[20];
	strcpy(name, "zhangsan");
//	ret = pthread_create(&pth, NULL, thread_handle, &num);
	ret = pthread_create(&pth, NULL, thread_handle, name);
	if (ret != 0)
	{
		fprintf(stderr, "创建线程失败\n");
		return -1;
	}

//	while (1);
	sleep(2);
//	printf("num = %d\n", num);
	printf("name = %s\n", name);
	return 0;
}
