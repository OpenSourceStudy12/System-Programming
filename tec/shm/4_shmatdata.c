#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

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
	char data[20] = "www.baidu.com";

	shmid = shmget(ftok("shm", 2), 1024*1024, IPC_CREAT | 0640);
	if (shmid < 0)
	{
		perror("shmget");
		return -1;
	}

	//映射共享内存段
	ptr = shmat(shmid, NULL, SHM_RND);//读写
	//if (ptr == (void*)-1)
	if ((int)ptr == -1)
	{
		fprintf(stderr, "映射共享内存失败\n");
		return -1;
	}

	stu.id = 1001;
	strcpy(stu.name, "zhang");
	stu.score = 80.5;

	memcpy(ptr, &stu, sizeof(stu));
	memcpy(ptr+sizeof(stu), data, sizeof(data));

	ret = shmctl(shmid, IPC_RMID, NULL);//将共享内存段标记为可删除， 当所有连接数都断开时才会真正的删除共享内存段
	if (ret < 0)
	{
		fprintf(stderr, "删除失败\n");
		return -1;
	}
	
	//断开映射
//	ret = shmdt(ptr);
	if (ret < 0){
		perror("shmdt");
		return -1;
	}
	for (int i = 0; i < 20; i++)
	{
		sleep(1);
	}
	return 0;
}
