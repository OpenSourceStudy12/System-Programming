#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>

#define SHM_SIZE 1024*1024

int main()
{
	int shmid;
	char *buff;

	shmid = shmget(ftok("./shm", 1), SHM_SIZE, IPC_CREAT | 0640);
	if (shmid < 0)
	{
		printf("get share memory failed\n");
		return -1;
	}

	buff = shmat(shmid, NULL, SHM_RND);
	if (buff == (void *)-1)
	{
		printf("at share memory failed\n");
		return -1;
	}

	sprintf(buff, "www.163.com\n");

	return 0;
}
