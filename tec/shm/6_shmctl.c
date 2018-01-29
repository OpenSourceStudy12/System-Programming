#include <stdio.h>
#include <sys/types.h>
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

	ret = shmctl(shmid, IPC_STAT, &shmbuf);// 读取共享内存内容到buf中 
	if (ret < 0)
	{
		printf("shmctl fail\n");
		return -1;
	}
	printf("shm_perm.__key = %d, shm_ctime = %ld\n", shmbuf.shm_perm.__key, shmbuf.shm_ctime);

	return 0;
}
