#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
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

struct teacher
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
		printf("创建消息队列失败\n");
		return -1;
	}

	struct student stu;
	stu.type = 2;
	stu.id = 101;
	sprintf(stu.name, "zhangsan");
	sprintf(stu.tel, "11111111111");

	struct teacher tea;
	tea.type = 1;
	tea.id = 201;
	sprintf(tea.name, "lisi");
	sprintf(tea.tel, "2222222222");

	ret = msgsnd(msgid, &stu, sizeof(stu) - sizeof(long), 0);
	if (ret < 0)
	{
		printf("发送失败\n");
		return -1;
	}

	ret = msgsnd(msgid, &tea, sizeof(tea) - sizeof(long), 0);
	if (ret < 0)
	{
		printf("发送失败\n");
		return -1;
	}

	printf("发送成功\n");
	return 0;
}
