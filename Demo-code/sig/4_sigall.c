#include <stdio.h>
#include <signal.h>

void handle(int argv)
{
	printf("recv a sig : %d\n", argv);
}

int main()
{
	for (int i = 1; i < 32; i++)
	{
		if (signal(i, handle) == SIG_ERR)
		{
			printf("注册信号失败:%d\n", i);
		}
	}

	while (1);
}
