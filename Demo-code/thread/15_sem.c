#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

sem_t sem[3];

void *thread_handle(void *argv)
{
	int id = (int)argv;

	for (int i = 0; i < 100; i++)
	{
		sem_wait(&sem[id]);
		printf("in thread[%d]\n", id);
//		usleep(500);
		if (id != 2)
			sem_post(&sem[id+1]);
		else
			sem_post(&sem[0]);

	}
}

int main()
{
	pthread_t pth[3];
	int ret;

	sem_init(sem, 0, 1);
	sem_init(sem+1, 0, 0);
	sem_init(sem+2, 0, 0);

	for (int i = 0; i < 3; i++)
	{
		ret = pthread_create(pth+i, NULL, thread_handle, (void*)i);
		if (ret != 0)
		{
			fprintf(stderr, "创建线程失败\n");
			return -1;
		}
	}

	for (int i = 0; i < 3; i++)
	{
		pthread_join(pth[i], NULL);
	}

	for (int i = 0; i < 3; i++)
		sem_destroy(sem+i);

	return 0;
}
