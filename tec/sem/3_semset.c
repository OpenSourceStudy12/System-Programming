#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int main()
{
	int semid, ret;
	unsigned short array[2];

	semid = semget(ftok("sem", 1), 2, IPC_CREAT | 0640);
	if (semid < 0)
	{
		fprintf(stderr, "创建或者获取信号量失败\n");
		return -1;
	}

	array[0] = 3;
	array[1] = 2;

	//ret = semctl(semid, 2, SETALL, array);
	ret = semctl(semid, 0, SETVAL, 6);
	if (ret < 0)
	{
		fprintf(stderr, "semctl fail\n");
		return -1;
	}

	return 0;
}
