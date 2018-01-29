#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int main()
{
	pid_t pid;

	pid = fork();
	if (pid == 0)
	{
		char *argv[] = {"NAME = ZHANG", "TEL = 123456", "SEX = M", NULL};
	//	char *argv[] = {"ls", "-l", NULL};
		printf("before exec\n");
	//	execvp("ls", argv);
		execle("./main", "main", NULL, argv);
		printf("after\n");
	}
	else if (pid > 0)
	{
		wait(NULL);
	}
}
