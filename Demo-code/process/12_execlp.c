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
		
		printf("before exel pid = %d\n", getpid());
		//execlp("ls", "ls", "-l", NULL);
		execlp("./test", "test", NULL);
		printf("after execl\n");
	}
	else if (pid > 0)
	{
		wait(NULL);
	}
}
