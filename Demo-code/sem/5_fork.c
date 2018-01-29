#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <string.h>

int main()
{
	int semid;
	int ret;
	int pid;
	struct sembuf sem;

	shmid = shmget();
	semid = semget(ftok("./sem", 2), 1, IPC_CREAT | 0640);
	if (semid < 0)
	{
		perror("semget");
		return -1;
	}

	ret = semctl(semid, 0, SETVAL, 0);
	if (ret < 0)
	{
		printf("初始化失败\n");
		return -1;
	}

//	pid = fork();
	if (!(pid = fork()))
	{
		sem.sem_num = 0;	//对下标为0的信号量进行操作
		sem.sem_op = -1;	//p操作
		sem.sem_flg = SEM_UNDO;	//进程结束恢复信号量初始值

		printf("before p\n");
		ret = semop(semid, &sem, 1);
		if (ret < 0)
		{
			printf("p操作失败\n");
			return -1;
		}
		printf("after p\n");

		return 0;
	}
	else if (pid > 0)
	{
		sem.sem_num = 0;
		sem.sem_op = 1;
		sem.sem_flg = SEM_UNDO;

		sleep(2);
		ret= semop(semid, &sem, 1);
		if (ret < 0)
		{
			printf("v操作失败\n");
			return -1;
		}

		wait(NULL);
		return 0;
	}
}
