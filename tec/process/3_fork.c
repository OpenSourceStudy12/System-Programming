#include <stdio.h>
#include <unistd.h>

int main()
{
	int num;
	pid_t pid;


	num = 10;
//	printf("in main\n");
	pid = fork();
	if (pid == 0)
	{
		while (1)
		{
//			printf("in child: getpid = %d, getppid = %d\n", getpid(), getppid());
			printf("in child: num = %d\n", num);
//			sleep(1);
		}
	}
	else if (pid > 0)
	{
		while (1)
		{
//			printf("in parent: pid = %d, getpid = %d\n", pid, getpid());
			printf("in parent: num = %d\n", num);
//			sleep(1);
		}
	}
	else 
	{
		printf("fork fail\n");
	}

	return 0;
}
