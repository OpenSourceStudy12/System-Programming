#include <stdio.h>
#include <assert.h>
#include <signal.h>
#include <unistd.h>

void handle(int argv)
{
	printf("捕捉到信号:%d\n", argv);
}

int main()
{
	if (signal(6, handle) == SIG_ERR)
	{
		printf("注册信号失败\n");
		return -1;
	}

	sleep(2);
	int i = 1;
	assert(i == 0);
	while (1);
}
