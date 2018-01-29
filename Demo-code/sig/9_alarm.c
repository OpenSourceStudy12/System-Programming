#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

void sig_handle(int argv)
{
	printf("recv a msg: %d\n", argv);
	alarm(2);
}
int main()
{
	if (signal(SIGALRM, sig_handle) == SIG_ERR)
	{
		printf("注册信号失败\n");
		return -1;
	}
	alarm(5);
	int i = 0;
	while (1)
	{
		printf("%d second\n", i);
		i++;
		sleep(1);
	}
}
