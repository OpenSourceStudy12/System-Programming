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
		fprintf(stderr, "获取或创建信号量集失败\n");
		return -1;
	}

	/*
	 *GETALL---->unsigned short *array, 第二个参数没有意义;
	 *SETALL---->unsigned short *array，第二个参数没有意义;
	 *SETVAL---->int, 第二个参数表示设置的信号量的下标;
	 *GETVAL---->没有第四个参数， 获取的信号量值保存在返回值中;
	 */
	//ret = semctl(semid, 2, GETALL, array);
	ret = semctl(semid, 1, GETVAL);
	if (ret < 0)
	{
		perror("semctl");
		return -1;
	}

//	printf("array[0] = %u, array[1] = %u\n", array[0], array[1]);
	printf("array[0] = %u\n", ret);
	return 0;
}
