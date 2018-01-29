#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

void sig_handle(int argv)
{
	printf("recv a sig:%d\n", argv);
	wait(NULL);
}

int main()
{
	int pid;

	pid = fork();
	if (pid == 0)
	{
		for (int i = 0; i < 2; i++)
		{
			printf("child process is running\n");
			sleep(1);
		}
	}
	else if (pid > 0)
	{
		if (signal(SIGCHLD, sig_handle) == SIG_ERR)
		{
			printf("注册信号失败\n");
			return -1;
		}
		while (1);
	}
}
