#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<errno.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<pthread.h>

//pthread_mutex_t mutex;
//pthread_cond_t cond;

void* thread_handle(void* argv)
{
	int fd_wr;
	int ret;
	char buff[200];

	fd_wr = open("myfifo2",O_WRONLY);//如果没有进程打开写，本进程将阻塞在此
	if(fd_wr < 0)
	{
		perror("open");
		return NULL;
	}

	while(1)
	{
		memset(buff,0,sizeof(buff));

//		pthread_mutex_lock(&mutex);
		ret = read(STDIN_FILENO,buff,sizeof(buff)-1);
//		pthread_mutex_unlock(&mutex);

		if(ret < 0)
		{
			perror("read");
			return NULL;
		}
		buff[ret-1] = '\0';
		ret = write(fd_wr,buff,strlen(buff));
		if(ret < 0)
		{
			perror("write");
			return NULL;
		}

		if(!strcmp(buff,"exit"))
			break;
//		printf("in fifo 2 \n");
//		usleep(200*1000);
	}
	close(fd_wr);
}

int main()
{
	int fd_re;
	int ret;
	char buff[200];
	pthread_t pth;

	ret = mkfifo("myfifo1",0640);
	if(ret < 0 && errno != EEXIST)
	{
		perror("fifo");
		return -1;
	}

	ret = mkfifo("myfifo2",0640);
	if(ret < 0 && errno != EEXIST)
	{
		perror("fifo");
		return -1;
	}

	ret = pthread_create(&pth,NULL,thread_handle,NULL);
	if(ret != 0)
	{
		perror("thread");
		return -1;
	}

	fd_re = open("myfifo1",O_RDONLY);//如果没有进程打开写，本进程将阻塞在此
	if(fd_re < 0)
	{
		perror("open");
		return -1;
	}

//
//	pthread_mutex_init(&mutex,NULL);
//	pthread_cond_init(&cond,NULL);

//	sleep(1);
	while(1)
	{
		memset(buff,0,sizeof(buff));

	//	pthread_mutex_lock(&mutex);
		ret = read(fd_re,buff,sizeof(buff)-1);
	//	pthread_mutex_unlock(&mutex);

		if(ret < 0)
		{
			perror("read");
			return -1;
		}
		buff[ret] = '\0';

		if(!strcmp(buff,"exit"))
			break;

		printf("receive a msg :%s\n",buff);
//		usleep(200*1000);
	}
	close(fd_re);

	pthread_join(pth,NULL);
//	pthread_mutex_destroy(&mutex);
//	pthread_cond_destroy(&cond);

	return 0;
}
