#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>
#include <assert.h>
#include <semaphore.h>

int fd[4];
sem_t sem[4];

void *thread_handle(void *argv)
{
	int id = (int)argv;
	int j = id;
	int ret;
	char text[10];
	memset(text, 0, sizeof(text));
	sprintf(text, "%c", 'a'+id);

	for (int i = 0; i < 50; i++)
	{
		sem_wait(&sem[id]);

		switch (id + i*3)
		{
			default :
				
			case 3:
				ret = write(fd[3], text, strlen(text));
				assert(ret > 0);
			case 2:
				ret = write(fd[2], text, strlen(text));
				assert(ret > 0);
			case 1:
				ret = write(fd[1], text, strlen(text));
				assert(ret > 0);
			case 0:
				ret = write(fd[0], text, strlen(text));
				assert(ret > 0);
				break;
		}
		if (id != 3)
			sem_post(&sem[id+1]);
		else
			sem_post(&sem[0]);
/*
		ret = write(fd[j], text, strlen(text));
		assert(ret > 0);
		--j;
		if (j < 0)
			j = 3;

		if (id != 3)
			sem_post(&sem[id+1]);
		else
			sem_post(&sem[0]);
*/
/*
		if (id == 0)
		{
			ret = write(fd[j], "a", 1);
			assert(ret > 0);
			--j;
			if (j < 0)
				j = 3;
			sem_post(&sem[id+1]);
		}
		else if (id == 1)
		{
			ret = write(fd[j], "b", 1);
			assert(ret > 0);
			--j;
			if (j < 0)
				j = 3;
			sem_post(&sem[id+1]);
		}
		else if (id == 2)
		{
			ret = write(fd[j], "c", 1);
			assert(ret > 0);
			--j;
			if (j < 0)
				j = 3;
			sem_post(&sem[id+1]);
		}
		else if (id == 3)
		{
			ret = write(fd[j], "d", 1);
			assert(ret > 0);
			--j;
			if (j < 0)
				j = 3;
			sem_post(&sem[0]);
		}
		*/
	}
}

int main()
{
	pthread_t pth[4];
	int ret;
	char name[10];

	for (int i = 0; i < 4; i++)
	{
		if (i == 0)
			sem_init(&sem[i], 0, 1);
		else
			sem_init(&sem[i], 0, 0);
	}

	for (int i = 0; i < 4; i++)
	{
		sprintf(name, "%c", 'A'+i);
		fd[i] = open(name, O_WRONLY | O_CREAT | O_TRUNC, 0640);
		assert(fd[i] > 0);
	}

	for (int i = 0; i < 4; i++)
	{
		ret = pthread_create(&pth[i], NULL, thread_handle, (void*)i);
		assert(ret == 0);
	}

	for (int i = 0; i < 4; i++)
	{
		pthread_join(pth[i], NULL);
	}

	for (int i = 0; i < 4; i++)
	{
		sem_destroy(&sem[i]);
		close(fd[i]);
	}

	return 0;
}
