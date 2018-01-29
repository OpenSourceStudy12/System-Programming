#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct student
{
	long type;
	char name[20];
	float score;
};

struct teacher
{
	long type;
	char name[20];
	char sex;
};

int main()
{
	int msgid, ret;
	struct student stu;
	struct teacher tea;
		         /*key键值*/		/*创建并赋予640权限*/
	msgid = msgget(ftok("msg", 1), IPC_CREAT | 0640);
	if (msgid < 0)
	{
		fprintf(stderr, "获取消息队列标识符失败\n");
		return -1;
	}

	stu.type = 2;
	strcpy(stu.name, "lisi");
	stu.score = 80;

	tea.type = 1;
	strcpy(tea.name, "zhangsan");
	tea.sex = 'F';

	/*msgget的返回值*/	    /*发送数据的大小*/  /*发送数据的形式：0-->阻塞*/
	|		          |                           |	
	ret = msgsnd(msgid, &stu, sizeof(stu) - sizeof(long), 0);
			      |
			/*所要发送的数据*/
	if (ret < 0)
	{
		perror("msgsnd");
		return -1;
	}
	ret = msgsnd(msgid, &tea, sizeof(tea) - sizeof(long), 0);
	if (ret < 0)
	{
		perror("msgsnd");
		return -1;
	}
	printf("发送成功\n");
	return 0;
}
