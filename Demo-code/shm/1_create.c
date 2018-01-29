#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>

#define SHM_SIZE 1024*1024

int main()
{
	int shmid;

	shmid = shmget(ftok("./shm", 1), SHM_SIZE, IPC_CREAT | 0640);
	if (shmid < 0)
	{
		printf("creat share memory failed\n");
		return -1;
	}

	printf("creat share memory sucess\n");
	return 0;
		
}
