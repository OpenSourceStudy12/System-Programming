#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void sig_handle(int argv)
{
	printf("recv a msg : %d\n", argv);
}

//abort() 向自己发送SIGABRT信号
//SIGABRT信号可以捕捉，但是仍会执行默认操作
int main()
{
	if (signal(SIGABRT, sig_handle) == SIG_ERR)
	{
		printf("捕捉信号失败\n");
		return -1;
	}

	for (int i = 0; i < 5; i++)
	{
		printf("the %d times\n", i);
		sleep(1);
	}

	abort();
	while (1);
	return 0;
}
