#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int main()
{
	int semid;

	semid = semget(ftok("sem", 1), /*信号量集中的信号个数*/2, IPC_CREAT | 0640);
	if (semid < 0)
	{
		fprintf(stderr, "创建或者获取信号量集失败\n");
		return -1;
	}

	printf("创建信号量集成功\n");
	return 0;
}
