#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>

int main()
{
	int semid;
	int ret;
	struct sembuf sem;

	semid = semget(ftok("./sem", 1), 3, IPC_CREAT | 0640);
	if (semid < 0)
	{
		perror("semget");
		return -1;
	}

	sem.sem_num = 1;
	sem.sem_op = 1;
	sem.sem_flg = SEM_UNDO;

	printf("before semop\n");
	ret = semop(semid, &sem, 1);
	if (ret < 0)
	{
		printf("p操作失败\n");
		return -1;
	}
	printf("after semop\n");

	return 0;
}
