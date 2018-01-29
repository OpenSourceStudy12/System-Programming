#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <string.h>

struct student
{
	int id;
	char name[20];
	char sex;
};
int main()
{
	int shmid;
	char *p;
	int ret;

	shmid = shmget(ftok("./shm", 1), 1024*1024, IPC_CREAT | 0640);
	if (shmid < 0)
	{
		perror("shmget");
		return -1;
	}

	p = shmat(shmid, NULL, SHM_RND);
	if ((int)p == -1)
	{
		printf("at memory failed\n");
		return -1;
	}

	printf("%s\n", p+sizeof(int));

	ret = shmdt(p);
	if (ret < 0)
	{
		printf("断开映射失败\n");
		return -1;
	}

	printf("%s\n", p+sizeof(int));
	return 0;
}
