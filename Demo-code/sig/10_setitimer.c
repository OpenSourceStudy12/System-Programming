#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>

void sig_handle(int argv)
{
	printf("recv a sig: %d\n", argv);
}

int main()
{
	if (signal(SIGALRM, sig_handle) == SIG_ERR)
	{
		printf("注册信号失败\n");
		return -1;
	}

	struct itimerval value;

	value.it_interval.tv_sec = 2;	//下一次触发事件的时间
	value.it_interval.tv_usec = 0;
	value.it_value.tv_sec = 5;		//第一次触发事件的时间
	value.it_value.tv_usec = 0;

	setitimer(ITIMER_REAL, &value, NULL);

	for (int i = 0; ; i++)
	{
		printf("the %d times\n", i);
		sleep(1);
	}
	while (1);
}
