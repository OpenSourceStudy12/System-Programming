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

	sembuff.sem_num = 0;
	sembuff.sem_op = 1;
	sembuff.sem_flg = IPC_NOWAIT;

	ret = semop(semid, &sembuff, 1);
	if (ret < 0)
	{
		perror("semop");
		return -1;
	}

	sleep(10);
	return 0;
}
