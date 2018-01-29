#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main()
{
	int msgid;

	msgid = msgget(ftok("msg", 1), IPC_CREAT | 0640);
	if (msgid < 0)
	{
		perror("msgget");
		return -1;
	}

	printf("creat success\n");
	return 0;
}
