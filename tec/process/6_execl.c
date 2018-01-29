#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

//exec：用其他的可执行程序替代当前进程
//调用exec前后，进程id号不变
//以l结尾的函数，默认是从当前文件夹下查找， 如果找不到，依旧执行之前的进程
int main()
{
	pid_t pid;

	pid = fork();
	if (pid == 0)
	{
		printf("before exec id = %d\n", getpid());
		execl("main", "main", "in", NULL);
	//	execl("ls", "ls", "-l", NULL);
		printf("after exec\n");

		exit(0);
	}
	else if (pid > 0)
	{
		wait(NULL);
	}
	else
	{
		perror("fork");
		return -1;
	}
}
