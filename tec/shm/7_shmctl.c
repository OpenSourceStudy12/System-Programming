#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main()
{
	int shmid, ret;
	struct shmid_ds shmbuf;

	shmid = shmget(ftok("shm", 2), 1024*1024, IPC_CREAT | 0640);
	if (shmid < 0)
	{
		fprintf(stderr, "创建或者获取共享内存失败\n");
		return -1;
	}

	ret = shmctl(shmid, IPC_RMID, NULL);//删除共享内存
	if (ret < 0)
	{
		printf("shmctl fail\n");
		return -1;
	}
	printf("删除成功\n");

	for (int i = 0; i < 20; i++)
	{
		sleep(1);
	}

	return 0;
}
