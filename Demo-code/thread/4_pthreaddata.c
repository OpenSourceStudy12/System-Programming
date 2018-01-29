#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

struct student
{
	int id;
	char name[20];
};

void *thread_handle(void *argv)
{
/*	struct student stu = *(struct student*)argv;
	printf("id = %d, name = %s\n", stu.id, stu.name);
	stu.id = 201;
	strcpy(stu.name, "lisi");
	*/
	int number = (int)argv;
	printf("number = %d\n", number);
	number = 200;
}

int main()
{
	pthread_t pth;
	struct student stu;
	int num = 10;

	stu.id = 101;
	strcpy(stu.name, "zhangsan");

//	int ret = pthread_create(&pth, NULL, thread_handle, &stu);
	int ret = pthread_create(&pth, NULL, thread_handle, (void*)num);
	if (ret != 0)
	{
		fprintf(stderr, "创建线程失败\n");
		return -1;
	}

	sleep(1);
//	printf("id = %d, name = %s\n", stu.id, stu.name);
	printf("num = %d\n", num);
	return 0;
}
