#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

void mysys(char *cmd)
{
	pid_t pid;
	pid = fork();

	if (pid == 0)
	{
		execlp(cmd, cmd, NULL);
	}
	else if (pid > 0)
	{
		wait(NULL);
	}
}
int main()
{
	char cmd[20];
	int ret = read(STDIN_FILENO, cmd, sizeof(cmd)-1);
	cmd[ret-1] = 0;

	mysys(cmd);
	return 0;
}
