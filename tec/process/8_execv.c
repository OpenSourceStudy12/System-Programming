#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

//参数以字符串数组形式传递
//execv-->execl
//execvp--->execlp
int main()
{
	pid_t pid;

	pid = fork();
	if (pid == 0)
	{
//		char *argv[] = {"main", NULL};
//错	char **argv = {"main", NULL};
		char *argv[] = {"ls", "-l", NULL};

		printf("before exec\n");
	//	execv("ls", argv);
		execvp("ls", argv);
		printf("after exec\n");

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
