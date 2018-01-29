#include<stdio.h>
#include<unistd.h>
#include<assert.h>
#include<pthread.h>

pthread_mutex_t mutex;
int food = 0;

void* thread_handle(void* argv)
{
	int id = (int)argv;
	while(1)
	{	
		pthread_mutex_lock(&mutex);
		if(id == 1)
		{
			printf("in shop   %d\n",++food);
		}
		else if(id == 2)
		{
			if(food > 0)
			{
				printf("in user1  %d\n",food--);
			}
			else
			{
				printf("in user1  %d\n",food);
			}
		}
		else 
		{
			if(food > 0)
			{
				printf("in user2  %d\n",food--);
			}
			else
			{
				printf("in user2  %d\n",food);
			}
		}
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}
}

int main()
{
	int ret;
	pthread_t pth1,pth2,pth3;

	pthread_mutex_init(&mutex,NULL);
	ret = pthread_create(&pth1,NULL,thread_handle,(void*)1);
	assert(ret == 0);

	ret = pthread_create(&pth2,NULL,thread_handle,(void*)2);
	assert(ret == 0);
	
	ret = pthread_create(&pth3,NULL,thread_handle,(void*)3);
	assert(ret == 0);

	pthread_join(pth1,NULL);
	pthread_join(pth2,NULL);

	pthread_mutex_destroy(&mutex);
	return 0;
}
