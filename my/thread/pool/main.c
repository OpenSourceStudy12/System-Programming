#include"pool.h"

void* handle(void * argv)
{
	int id = *(int *)argv;

	printf("in thread[%d]\n",id);
	usleep(500*1000);
}

int main()
{
	int ret;
	ret = pool_init(10);
	if(ret < 0)
	{
		fprintf(stderr,"pool init fail\n");
		return -1;
	}
	sleep(2);//延时等子线程创建完

	for(int i=0;i<5;i++)
	{
		func_add(handle,&i);
	}

	sleep(5);//延时令子线程执行
	pool_destroy();
	return 0;
}
