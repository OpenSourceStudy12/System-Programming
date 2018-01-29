#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main()
{
	int ret;
	
	ret = msgget(1, IPC_CREAT | 0640);
	if (ret < 0)
	{
		fprintf(stderr, "creat msg fail\n");
		return -1;
	}

	printf("creat success\n");
	return 0;
}
