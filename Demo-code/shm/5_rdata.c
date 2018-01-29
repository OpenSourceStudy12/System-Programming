#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>

struct student
{
	int id;
	char name[20];
	char sex;
};

int main()
{
	int shmid;
//	struct student *p;
	void *p;
	struct student stu;
	char data[20];

	shmid = shmget(ftok("./shm", 1), 1024*1024, IPC_CREAT | 0640);
	if (shmid < 0)
	{
		perror("shmget");
		return -1;
	}

	p = shmat(shmid, NULL, SHM_RND);
	if (p == (void*)-1)
	{
		perror("shmat");
		return -1;
	}

	memcpy(&stu, p, sizeof(stu));
	memcpy(data, p+sizeof(stu), sizeof(data));
//	printf("id = %d, name = %s, sex = %c\n", p->id, p->name, p->sex);
	printf("id = %d, name = %s, sex = %c, data = %s\n", stu.id, stu.name, stu.sex, data);
	
	int ret = shmctl(shmid, IPC_RMID, NULL);
	if (ret < 0)
	{
		perror("shmctl");
		return -1;
	}

	ret = shmdt(p);
	if (ret < 0)
	{
		printf("断开失败\n");
		return -1;
	}
	while (1);
	return 0;
}
