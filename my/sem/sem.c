#include<stdio.h>
#include<sys/tepys.h>
#include<sys/ipc.h>
#include<sys/sem.h>

int main()
{
	int semid,ret;
	pid_t pid;
	struct sembuf sembuff;

	semid = semget(ftok("sem",1),2,IPC_CREAT|0640);//创建获取信号量
	if(semid < 0)
	{
		perror("semget");
		return -1;
	}

	ret = semctl(semid,0,SETVAL,0);//设置信号量(0)初值为0
	if(ret < 0)
	{
		perror("semctl");
		return -1;
	}
	
	pid = fork();//分列进程
	if(pid == 0)
	{
		perror("fork");
		return -1;
	}
	else if(pid == 0)
	{
		semnuff.num = 0;
		sembuff.sem_op = 1;//信号量加1
		sembuff.sem_flg = SEM_UNDO;//撤销上次操作

	}
	else
	{
		sembuff.num = 0;
		sembuff.sem_op = -1;
		sembuff.sem_flg = SEM_UNDO;
	}

			
}
