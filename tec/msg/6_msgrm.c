#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main()
{
	int msgid, ret;

	msgid = msgget(ftok("msg", 1), IPC_CREAT | 0640);
	if (msgid < 0)
	{
		perror("msgget");
		return -1;
	}

	ret = msgctl(msgid, IPC_RMID, NULL);
	if (ret < 0)
	{
		perror("msgctl");
		return -1;
	}
	return 0;
}
