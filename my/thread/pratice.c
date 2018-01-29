#include<stdio.h>
#include<assert.h>
#include<semaphore.h>
#include<pthread.h>
#include<fcntl.h>
#include<unistd.h>

//四个进程 0 1 2 3分别只能写字符 'a' 'b' 'c' 'd'
//利用四个进程向4个文件 A B C D中写数据，使得数据格式如下:
//A: abcdabcdabcdabcdabcd---
//B: bcdabcdabcdabcdabcda---
//C: cdabcdabcdabcdabcdab---
//D: dabcdabcdabcdabcdabc---

sem_t sem[4];//四个信号量
int fd[4];//文件描述符
char s[4]={'a','b','c','d'};

void* thread_handle(void* argv)
{
	int id = (int)argv;
	int ret,n = id;
//	int order[4];
/*
	if(id == 0)
	{
		 order={0,3,2,1};
	}
	else if(id == 1)
	{
		 order[]={1,0,3,2};
	}
	else if(id == 2)
	{
		 order[]={2,1,0,3};
	}
	else 
	{
		 order[]={3,2,1,0};
	}
*/
	for(int i=0;i<20;i++)
	{	
		sem_wait(sem+id);//信号量-1   <0 阻塞，>=0 执行  在每个线程的sem_wait和sem_post之间的程序执行期间不会有其它线程的程序执行

		if(n < 0)
			n = 3;

		ret = write(fd[n--],s+id,1);
		if(ret < 0)
			return NULL;

		if(id == 3)
			sem_post(sem);//线程3执行完后(阻塞)，切换到线程0
		else
			sem_post(sem+id+1);//当前线程结束后(阻塞)，后面的线程执行
	}
}

int fileopen(int*,char**,int);//打开文件，返回文件描述符

int main()
{
	int ret;
	pthread_t pth[4];
	char* file[4] = {"A","B","C","D"};//文件名

	ret = fileopen(fd,file,4);
	if(ret < 0)
	{
		fprintf(stderr,"open file fail");
		return -1;
	}
	//创建4个线程
	for(int i=0;i<4;i++)
	{
		pthread_create(pth+i,NULL,thread_handle,(void*)i);
	}
	//初始化4个信号量
	for(int i=0;i<4;i++)
	{
		if(i == 0)
		{
			sem_init(sem+i,0,1);
			continue;
		}
		sem_init(sem+i,0,0);
	}
	//回收线程资源
	for(int i=0;i<4;i++)
	{
		pthread_join(pth[i],NULL);
	}
	//销毁信号量
	for(int i=0;i<4;i++)
	{
		sem_destroy(sem+i);
	}
	return 0;
}

int fileopen(int* fd,char** file,int len)
{
	for(int i=0;i<len;i++)
	{
		fd[i] = open(file[i],O_WRONLY | O_CREAT | O_TRUNC,0640);//以只读|创建|清空方式打开文件
		if(fd[i] < 0)
			return -1;
	}
	return 0;
}
