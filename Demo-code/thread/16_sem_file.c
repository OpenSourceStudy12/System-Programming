#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <assert.h>
#include <semaphore.h>

sem_t sem[2];

void *create_handle(void *argv)
{
	char name[10];
	int ret;
	int id = (int)argv;

	for (int i = 0; i < 10; i++)
	{
		sem_wait(&sem[0]);
//		memset(name, 0, sizeof(name));
		bzero(name, sizeof(name));
		sprintf(name, "file%d", i);
		ret = creat(name, 0640);
		if (ret < 0)
			pthread_exit((void *)1);
		
		sleep(1);
		sem_post(&sem[1]);
	}

	pthread_exit((void *)0);
}

void *remove_handle(void *argv)
{
	char name[10];
	int ret;
	int id = (int)argv;

	for (int i = 0; i < 10; i++)
	{
		sem_wait(&sem[1]);
//		memset(name, 0, sizeof(name));
		bzero(name, sizeof(name));
		sprintf(name, "file%d", i);
		ret = remove(name);
		if (ret < 0)
			pthread_exit((void *)1);

		sleep(1);
		sem_post(&sem[0]);
	}

	pthread_exit((void *)0);
}

int main()
{
	pthread_t pth1, pth2;
	int ret;

	sem_init(sem, 0, 1);
	sem_init(sem+1, 0, 0);

	ret = pthread_create(&pth1, NULL, create_handle, (void*)0);
	assert(ret == 0);
	ret = pthread_create(&pth2, NULL, remove_handle, (void*)1);
	assert(ret == 0);

	pthread_join(pth1, NULL);
	pthread_join(pth2, NULL);

	sem_destroy(&sem[0]);
	sem_destroy(&sem[1]);
	return 0;
}
