#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>

int main()
{
	int msgid;

	msgid = msgget(ftok("./io", 1), IPC_CREAT | 0640);
	if (msgid < 0)
	{
		printf("创建消息队列失败\n");
		return -1;
	}

	return 0;
}
