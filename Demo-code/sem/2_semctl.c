#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>

/*
 *1.联合体不能作为函数的入参
 *2.如果设置GETALL -> unsigned short
 *3.GETVAL -> 返回值
 */
int main()
{
	int semid;
	unsigned short array[3];
	int ret;

	semid = semget(ftok("./sem", 1), 3, IPC_CREAT | 0640);
	if (semid < 0)
	{
		printf("获取信号量失败\n");
		return -1;
	}

	ret = semctl(semid, 3, GETALL, array);
	if (ret < 0)
	{
		perror("semctl");
		return -1;
	}

	printf("a1 = %d, a2 = %d, a3 = %d\n", array[0], array[1], array[2]);
	ret = semctl(semid, 0, GETVAL);
	ret = semctl(semid, 1, GETVAL);
	printf("ret = %d\n", ret);

	return 0;
}
