#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void mysys(char *argv0, char *argv1)
{
	pid_t pid;

	pid = fork();
	if (pid == 0)
	{
		execlp(argv0, argv0, argv1, NULL);
		exit(0);
	}
	else if (pid > 0)
	{
		wait(NULL);
	}
	else 
		perror("fork");
}

int main()
{
	mysys("ls", "-l");

	printf("after system\n");

	return 0;
}
