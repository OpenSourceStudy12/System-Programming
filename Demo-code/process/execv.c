#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

//execv 只是把execl的后面的参数保存到了字符串数组中
int main()
{
	pid_t pid;

	pid = fork();
	if (pid == 0)
	{
//		char *argv[] = {"test", NULL};
		char *argv[] = {"ls", "-al", NULL};
		printf("pdi = %d, before execv\n", getpid());
		execv("/bin/ls", argv);
//		execv("./test", argv);
		//execl("./test", "test", NULL);
		printf("after execv\n");
	}
	else if (pid > 0)
	{
		waitpid(-1, NULL, 0);
	}
	return 0;
}
