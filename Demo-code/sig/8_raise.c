#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

void sig_handle(int argv)
{
	printf("recv a msg: %d\n", argv);
}

int main()
{
	if (signal(SIGINT, sig_handle) == SIG_ERR)
	{
		printf("注册信号失败\n");
		return -1;
	}

	raise(SIGINT);
	for (int i = 0; i < 5; i++)
	{
		sleep(1);
		printf("%d second\n", i);
	}

	if (signal(SIGINT, SIG_DFL) == SIG_ERR)
	{
		printf("修改默认处理失败\n");
		return -1;
	}

	raise(SIGINT);

	while (1);
}
