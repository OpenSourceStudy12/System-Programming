#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>

struct student
{
	long type;
	int id;
	char name[20];
	char tel[11];
};

int main()
{
	int msgid;
	int ret;

	msgid = msgget(ftok("./test", 1), IPC_CREAT | 0640);
	if (msgid < 0)
	{
		printf("获取消息队列失败\n");
		return -1;
	}
/*
	struct student stu;
	memset(&stu, 0, sizeof(stu));
	ret = msgrcv(msgid, &stu, sizeof(stu)-sizeof(long), 1, 0);
	if (ret < 0)
	{
		printf("接受消息失败\n");
		return -1;
	}

	printf("ret = %d, stu.id = %d, stu.name = %s, stu.tel = %s\n", ret, stu.id, stu.name, stu.tel);

	memset(&stu, 0, sizeof(stu));
	ret = msgrcv(msgid, &stu, sizeof(stu)-sizeof(long), 2, 0);
	if (ret < 0)
	{
		printf("读取消息失败\n");
		return -1;
	}

	printf("ret = %d, tea.id = %d, tea.name = %s, tea.tel = %s\n", ret, stu.id, stu.name, stu.tel);
*/
	ret = msgctl(msgid, IPC_RMID, NULL);
	if (ret < 0)
	{
		printf("删除消息队列失败\n");
		return -1;
	}
	perror("msgctl");
	return 0;
}
