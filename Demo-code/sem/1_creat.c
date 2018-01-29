#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>

int main()
{
	int semid;
	unsigned short array[3];
	int ret;

	semid = semget(ftok("./sem", 1), 3, IPC_CREAT | 0640);
	if (semid < 0)
	{
		perror("semget");
		return -1;
	}

	printf("创建成功\n");
	array[0] = 1;
	array[1] = 2;
	array[2] = 2;

	ret = semctl(semid, 3, SETALL, array);
	if (ret < 0)
	{
		printf("初始化信号量失败\n");
		return -1;
	}
	
	memset(array, 0, sizeof(array));
	ret = semctl(semid, 3, GETALL, array);
	if (ret < 0)
	{
		printf("获取失败\n");
		return -1;
	}
	printf("a1 = %d, a2 = %d, a3 = %d\n", array[0], array[1], array[2]);

	return 0;
}
