#include <stdio.h>
#include <signal.h>

void sig_handle(int argv)
{
	printf("收到信号:%d\n", argv);
}

int main()
{
	if (signal(SIGSEGV, sig_handle) == SIG_ERR)
	{
		printf("信号注册失败\n");
		return -1;
	}

	int *p;
	*p = 4;
	while (1);
}
