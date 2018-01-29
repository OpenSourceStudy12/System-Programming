#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void sig_handle(int argv)
{
	printf("recv a sig: %d\n", argv);
}

int main()
{
	if (signal(SIGTSTP, sig_handle) == SIG_ERR)
	{
		printf("注册信号失败\n");
		return -1;
	}
	for (int i = 0; i < 5; i++)
	{
		sleep(1);
		printf("%d seconds\n", i);
	}

	kill(getpid(), SIGTSTP);
	while (1);
}
