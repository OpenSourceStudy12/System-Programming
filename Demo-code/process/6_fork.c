#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

int main()
{
	pid_t pid;

	pid = fork();

	if (pid == 0)
	{
		printf("in child pid = %d\n", getpid());
		exit(0);
	}
	else if (pid > 0)
	{
		while (1);
	}
	return 0;
}
