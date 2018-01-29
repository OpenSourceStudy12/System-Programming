#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
	pid_t pid;

	pid = fork();
	
	if (pid == 0)
//	if (!fork())
//	if (!(pid = fork()))
	{
		printf("in B\n");
	}
	else if (pid > 0)
	{
		pid = fork();
		if (pid == 0)
		{
			pid = fork();
			if (pid == 0)
			{
				printf("in F\n");
			}
			else if (pid > 0)
			{
				printf("in C\n");
			}
			else 
				exit(0);
		}
		else if (pid > 0)
		{
			pid = fork();
			if (pid == 0)
			{
				printf("in D\n");
			}
			else if (pid > 0)
			{
				printf("A\n");
			}
			else
				exit(0);
		}
		else
			exit(0);
	}
	else 
		exit(0);

	while (1);
	return 0;
}
