#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
	pid_t pid;

	pid = fork();

	if (pid == 0)
	{
		//in child process
		for (int i = 0; i < 5; i++)
		{
			printf("child process is running\n");
//			sleep(1);
		}
//		exit(0);
		return 0;
	}
	else if (pid > 0)
	{
		for (int i = 0; i < 10; i++)
		{
			printf("parent process is running\n");
//			sleep(1);
		}

		return 0;
	}
}
