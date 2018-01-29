#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
	pid_t pid;

	pid = fork();
	if (pid == 0)
	{
		for (int i = 0; i < 5; i++)
		{
			printf("child processp[%d] is running\n", getpid());
			sleep(1);
		}

		return 0;
	}
	else if (pid > 0)
	{
		printf("befor wait\n");
//		pid = waitpid(-1, NULL, 0);		wait(NULL);
//		waitpid(pid, NULL, 0);
		pid = waitpid(-1, NULL, WNOHANG);
		printf("after wait pid = %d\n", pid);
		return 0;
	}
}
