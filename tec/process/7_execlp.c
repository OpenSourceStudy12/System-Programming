#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

//以p结尾的函数，默认从环境变量PATH所制定的文件夹当中查找
int main()
{
	pid_t pid;

	pid = fork();
	if (pid == 0)
	{
		printf("in child: id = %d\n", getpid());
		execlp("ls", "ls", "-al", NULL);
	//	execlp("./main", "main", NULL);
		printf("after execlp\n");
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
