#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

static int i = 10;
int main()
{
	//进程从fork()开始一个分裂一个子进程， 子进程与父进程具有一模一样的资源
	//但是子进程从frok()开始运行
	pid_t pid;
	printf("process is running\n");
	i+=10;
	pid = fork();

	if (pid == 0)
	{
		i++;
		printf("in child i = %d\n", i);
		printf("in child pid = %d\n", getpid());
		printf("in child ppid = %d\n", getppid());
	}
	else if (pid > 0)
	{
		printf("in parent i = %d\n", i);
		printf("in parent pid = %d\n", getpid());
		printf("in parent ppid = %d\n", getppid());
		sleep(1);
	}

	return 0;
}
