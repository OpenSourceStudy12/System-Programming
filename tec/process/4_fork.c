#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
	pid_t pid;

	if (!(pid = fork()))
	{
		for (int i = 0; i < 10; i++)
		{
			printf("in child pid = %d, ppid = %d\n", getpid(), getppid());
			usleep(500*1000);
		}

		return 0;
	}
	else if (pid > 0)
	{
		for (int i = 0; i < 5; i++)
		{
			printf("in parent pid = %d\n", getpid());
			usleep(500*1000);
		}

		return 0;
//		exit(0);
	}
	else
	{
		printf("error\n");
		return -1;
	}
}
