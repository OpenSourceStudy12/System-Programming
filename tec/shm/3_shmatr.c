#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 1024*1024

int main()
{
	int shmid, ret;
	char *p;

	shmid = shmget(ftok("shm", 1), SHM_SIZE, IPC_CREAT | 0640);
	if (shmid < 0)
	{
		fprintf(stderr, "创建或者获取共享内存失败\n");
		return -1;
	}

	/*映射*/
	p = shmat(shmid, NULL, SHM_RDONLY);//只读
	if (p == (void*)-1)
	{
		fprintf(stderr, "映射失败\n");
		return -1;
	}

	printf("shm: %s\n", p);
	ret = shmdt(p);//断开链接
	if (ret < 0)
	{
		perror("shmdt");
		return -1;
	}
	
	return 0;
}
