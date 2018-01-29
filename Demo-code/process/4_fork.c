#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
	pid_t pid;

	pid = fork();
	if (pid == 0)
	{
		//in child
		printf("pid = %d\n", getpid());
		while(1);
	}
	else if (pid > 0)
	{
		//in parent
		printf("ppid = %d\n", getpid());
		while (1);
	}

	return 0;
}
