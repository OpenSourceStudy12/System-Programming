#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

struct student 
{
	int id;
	char name[20];
	float score;
};

int main()
{
	int shmid, ret;
	struct student stu;
	void *ptr;
	char data[20];

	shmid = shmget(ftok("shm", 2), 1024*1024, IPC_CREAT | 0640);
	if (shmid < 0)
	{
		perror("shmget");
		return -1;
	}

	ptr = shmat(shmid, NULL, SHM_RND);
	//if (ptr == (void*)-1)
	if ((int)ptr == -1)
	{
		fprintf(stderr, "映射共享内存失败\n");
		return -1;
	}

	memcpy(&stu, ptr, sizeof(stu));
	memcpy(data, ptr+sizeof(stu), sizeof(data));
	printf("id = %d, name = %s, score = %.1f, data = %s\n", stu.id, stu.name, stu.score, data);
	return 0;
}
