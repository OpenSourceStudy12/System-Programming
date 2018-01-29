#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>
#include<assert.h>
#include<semaphore.h>

sem_t sem[3];

void* thread_handle(void* argv)
{
	int id = (int)argv;

	for(int i=0;i<100;i++)
	{
		sem_wait(sem+id);
		printf("thread[%d] is running\n",id);
		if(id == 2)
		{
			sem_post(sem);
		}
		else
		{
			sem_post(sem+id+1);
		}
	}
}

int main()
{
	pthread_t pth[3];
	int ret;

	for(int i=0;i<3;i++)
	{
		if(i == 0)
		{
			sem_init(sem+i,0,1);
			continue;
		}
		sem_init(sem+i,0,0);
	}

	for(int i=0;i<3;i++)
	{
		ret = pthread_create(pth+i,NULL,thread_handle,(void*)i);
		assert(ret == 0);
	}

	for(int i=0;i<3;i++)
	{	
		pthread_join(pth[i],NULL);
	}

	for(int i=0;i<3;i++)
	{
		sem_destroy(sem+i);
	}
}
