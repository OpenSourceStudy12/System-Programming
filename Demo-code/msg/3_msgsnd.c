#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>

struct buf
{
	long type;
	char text[200];
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

	struct buf msg;
	msg.type = 1;
	sprintf(msg.text, "www.baidu.com");

	ret = msgsnd(msgid, &msg, sizeof(msg) - sizeof(long), 0);
	if (ret < 0)
	{
		printf("发送失败\n");
		return -1;
	}

	printf("发送成功\n");
	return 0;
}
