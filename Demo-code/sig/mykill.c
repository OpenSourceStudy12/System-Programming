#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		printf("参数有误\n");
		return -1;
	}
/*
	int pid = atoi(argv[2]);
	//sscanf(argv[2], "%d", &pid);
	int sig = 0 - atoi(argv[1]);
	//sscanf(argv[1], "%*c%d", &sig);
	int ret = kill(pid, sig);
	if (ret < 0)
	{
		printf("输入参数有误\n");
		return -1;
	}
*/
	execlp("kill", "kill", argv[1], argv[2], NULL);
	return 0;
}
