#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct teacher
{
	long type;
	char name[20];
	char sex;
};

struct student
{
	long type;
	char name[20];
	float score;
};

int main()
{
	int msgid, ret;
	struct teacher tea;
	struct student stu;

	msgid = msgget(ftok("msg", 1), IPC_CREAT | 0640);
	if (msgid < 0)
	{
		fprintf(stderr, "获取消息队列标识符失败\n");
		return -1;
	}
	
	/*所要接受的消息的类型*/
	ret = msgrcv(msgid, &tea, sizeof(tea)-sizeof(long), -3, 0);
	if (ret < 0)
	{
		perror("msgsnd");
		return -1;
	}

	ret = msgrcv(msgid, &stu, sizeof(stu)-sizeof(long), 0, 0);
	if (ret < 0)
	{
		perror("msgsnd");
		return -1;
	}

	printf("in tea: name = %s, sex = %c\n", tea.name, tea.sex);
	printf("name = %s, score = %.1f\n", stu.name, stu.score);
	return 0;
}
