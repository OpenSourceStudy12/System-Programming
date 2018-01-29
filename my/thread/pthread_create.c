#include<stdio.h>
#include<unistd.h>
#include<assert.h>
#include<pthread.h>

void* thread_handle(void* argv)
{
//	static int num = 10;
	int num = 10;
	char* str = argv;
	printf("in thread %s\n",str);
	sleep(1);
//	pthread_exit(&num);
	pthread_exit((void*)num);
}

int main()
{
	pthread_t pth;
	int ret;
	int* status;
	char* str = "hello word";

	ret = pthread_create(&pth,NULL,thread_handle,str);
	assert(ret == 0);
	pthread_join(pth,(void**)&status);
//	printf("in main thread,status = %d\n",*status);
	printf("in main thread,status = %d\n",(int)status);
	
}
