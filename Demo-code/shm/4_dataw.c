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
	char data[20];
	struct student stu;
	void *p;
//	struct student *p;

	shmid = shmget(ftok("./shm", 1), 1024*1024, IPC_CREAT|0640);
	if (shmid < 0)
	{
		perror("shmget");
		return -1;
	}

	memset(&stu, 0, sizeof(stu));
	memset(data, 0, sizeof(data));
	stu.id = 1001;
	strcpy(stu.name, "zhangsan");
	stu.sex = 'M';
	strcpy(data, "no data");

	p = shmat(shmid, NULL, SHM_RND);
	if (p == (void *)-1)
	{
		perror("shmat");
		return -1;
	}

//	sprintf(p, "%d%s%c", stu.id, stu.name, stu.sex);
	memcpy(p, &stu, sizeof(stu));
//	p->id = 1001;
//	strcpy(p->name, "zhangsan");
//	p->sex = 'M';
	memcpy(p+sizeof(stu), data, sizeof(data));

	printf("写入成功\n");
	return 0;
}
