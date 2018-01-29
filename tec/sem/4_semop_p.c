#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int main()
{
	int semid, ret;
	struct sembuf sembuff;

	semid = semget(ftok("sem", 1), 2, IPC_CREAT | 0640);
	if (semid < 0)
	{
		fprintf(stderr, "创建或者获取信号量数组失败\n");
		return -1;
	}

	ret = semctl(semid, 0, SETVAL, 0);
	if (ret < 0)
	{
		fprintf(stderr, "初始化信号量失败\n");
		return -1;
	}

	sembuff.sem_num = 0;
	sembuff.sem_op = -2;
	sembuff.sem_flg = SEM_UNDO;
	ret = semop(semid, &sembuff, 1);
	if (ret < 0)
	{
		perror("semop");
		return -1;
	}

//	sleep(5);
	return 0;
}
