#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
	pid_t pid;

	pid = fork();

	if (pid == 0)
	{
		//子进程
		while (1)
		{
			printf("this is child: pid = %d\n", pid);
			sleep(1);
		}
	}
	else if (pid > 0)
	{
		//父进程
		while (1)
		{
			printf("in parent: pid = %d\n", pid);
			sleep(1);
		}
	}
	else 
		printf("fork fail\n");

	return 0;
}
