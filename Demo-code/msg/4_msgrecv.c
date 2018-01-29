#include <stdio.h>
#include <unistd.h>
#include <string.h>
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
		printf("获取消息队列失败\n");
		return -1;
	}

	struct buf msg;
	memset(&msg, 0, sizeof(msg));
	ret = msgrcv(msgid, &msg, sizeof(msg)-sizeof(long), 1, 0);
	if (ret < 0)
	{
		printf("接受消息失败\n");
		return -1;
	}

	printf("ret = %d, msg.text = %s\n", ret, msg.text);
	return 0;
}
