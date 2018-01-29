#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

struct student 
{
	int id;
	char name[20];
};

void *thread_handle(void *argv)
{
//	int num = *(int*)argv;
//	int num = (int)argv;
//	char str[20];
//	strcpy(str, argv);
//	strcpy(str, "1234567890");
//	char *str = argv;
//	strcpy(str, "654321");
//	num = 100;

	struct student stu = *(struct student*)argv;
	stu.id = 1002;
	strcpy(stu.name, "wang");
//	printf("in thread_handle: num = %d\n", num);
//	printf("in thread_handle: str = %s\n", str);
	printf("in thread_handle: id = %d, name = %s\n", stu.id, stu.name);
	
	return NULL;
}

int main()
{
	pthread_t pth;
	int ret;
	int num;
	char str[20] = "hello world";
	struct student stu;
	stu.id = 1001;
	strcpy(stu.name, "zhang");

	num = 10;
//	ret = pthread_create(&pth, NULL, thread_handle, (void*)num);
//	ret = pthread_create(&pth, NULL, thread_handle, str);
	ret = pthread_create(&pth, NULL, thread_handle, &stu);
	assert(ret == 0);

	pthread_join(pth, NULL);

//	printf("in main thread: num = %d\n", num);
//	printf("in main thread: str = %s\n", str);
	printf("in main thread: id = %d, name = %s\n", stu.id, stu.name);
	return 0;
}
