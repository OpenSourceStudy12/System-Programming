#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

//execl: 用其他的可执行程序代替当前的子进程。
//如果没有加路径， 默认是从当前文件夹查找可执行程序
int main()
{
	pid_t pid;

	pid = fork();
	if (pid == 0)
	{
		
		printf("before exel pid = %d\n", getpid());
//		execl("./test", "test", NULL);
		execl("/bin/ls", "ls", "-l", NULL);
		printf("after execl\n");
	}
	else if (pid > 0)
	{
		wait(NULL);
		printf("end\n");
	}
}
