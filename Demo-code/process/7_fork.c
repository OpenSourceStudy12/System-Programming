#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
	pid_t pid;

	pid = fork();
	if (pid == 0)
	{
		printf("pid = %d\n", getpid());
		while(1);
	}
	else if (pid > 0)
	{
		printf("pid = %d\n", getpid());
		pid = fork();

		if (pid == 0)
		{
			pid = fork();
			if (pid == 0)
			{
				printf("pid = %d\n", getpid());
				while (1);
			}
			else if (pid > 0)
			{
				printf("pid = %d\n", getpid());
				while (1);
			}
		}
		else if (pid > 0)
		{
			pid = fork();
			if (pid == 0)
			{
				printf("pid = %d\n", getpid());
				while (1);
			}
			else if (pid > 0)
			{
				while (1);
			}
		}
	}

	return 0;
}
