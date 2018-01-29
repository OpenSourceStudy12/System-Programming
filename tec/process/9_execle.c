#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
	pid_t pid;

	pid = fork();
	if (pid == 0)
	{
		char *environ[] = {"NAME = zhangsan", "HOME = 123456", "LISEN = 6544321", NULL};

		printf("in execle.c\n");
		execle("./environ", "environ", NULL, environ);
		printf("after execle\n");

		return 0;
	}
	else if (pid > 0)
	{
		wait(NULL);

		return 0;
	}
	else 
	{
		perror("fork");
		return -1;
	}
}
