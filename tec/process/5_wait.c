#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

//wait 返回回收的子进程的id
//wait 接受子进程的资源， 调用时会阻塞
//wait 可以接受exit的返回值，需要借用两个宏函数
int main()
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid == 0)
	{
		for (int i = 0; i < 3; i++)
		{
			printf("in child: pid = %d\n", getpid());
	//		sleep(1);
		}

		exit(254);
//		return 0;
	}
	else if (pid > 0)
	{
		int id;

		printf("before wait\n");
	//	id = wait(&status);
	//	id = waitpid(pid, &status, 0);
		id = waitpid(-1, &status, 0);
		printf("after wait: id = %d\n", id);
		if (WIFEXITED(status))
			printf("status = %d\n", WEXITSTATUS(status));

//		while (1);
	}
}
