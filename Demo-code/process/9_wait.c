#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

/*
 * 1.用于回收子进程的资源
 * 2.wait是一个阻塞函数，一直到某个子进程退出才会解除阻塞
 * 3.返回值为回收的子进程的id
 * 4.wait 可以接受子进程退出时调用exit返回的值 但是需要两个宏 而且接受值只能是0～255
 */
int main()
{
	pid_t pid;

	pid = fork();

	if (pid == 0)
	{
		//in child process
		for (int i = 0; i < 5; i++)
		{
			printf("child process[%d] is running\n", getpid());
			sleep(1);
		}
		exit(256);
//		return 0;
	}
	else if (pid > 0)
	{
		int status;
		pid_t pid2;
//		sleep(10);

		printf("before wait\n");
		pid2 = wait(&status);
		printf("after wait\n");
		
//		printf("pid = %d\n", pid2);
		if (WIFEXITED(status))
			printf("status = %d\n", WEXITSTATUS(status));
		return 0;
	}
}
