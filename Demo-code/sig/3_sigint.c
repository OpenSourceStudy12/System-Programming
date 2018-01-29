#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main()
{
	if (signal(SIGINT, SIG_IGN) == SIG_ERR)
	{
		fprintf(stderr, "忽略信号失败\n");
		return -1;
	}
	if (signal(SIGTSTP, SIG_IGN) == SIG_ERR)
	{
		fprintf(stderr, "忽略信号失败\n");
		return -1;
	}

	int seconds = 0;
	for (;;)
	{
		printf("%d times\n", seconds);
		sleep(1);
		seconds++;

		if (seconds == 10)
		{
			if (signal(SIGINT, SIG_DFL) == SIG_ERR)
			{
				printf("注册信号失败\n");
				return -1;
			}
			if (signal(SIGTSTP, SIG_DFL) == SIG_ERR)
			{
				printf("注册信号失败失败\n");
				return -1;
			}
		}
	}
}
