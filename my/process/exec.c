#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>

int main()
{
	pid_t pid;
	pid = fork();

	if(pid == 0)
	{
		printf("in current  pid = %d\n",getpid());
		//execl("/bin/ls","ls","-l",NULL);
		execlp("ls","ls","/",NULL);
		printf("in other \n");
		return 0;
	}
	else if(pid > 0)
	{
		wait(NULL);
		return 0;
	}
	else
	{
		perror("error");
		return -1;
	}
}
