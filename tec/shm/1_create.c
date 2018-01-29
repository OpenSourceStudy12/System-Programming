#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 1024*1024
int main()
{
	int shmid;
	int ret;

//	shmid = shmget(ftok("shm", 2), SHM_SIZE, IPC_CREAT | 0640);
	shmid = shmget(ftok("shm", 1), SHM_SIZE, IPC_CREAT | IPC_EXCL | 0640);
	if (shmid < 0)
	{
		perror("shmget");
		return -1;
	}

	printf("创建共享内存成功\n");
	return 0;
}
