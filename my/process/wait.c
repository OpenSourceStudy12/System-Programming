#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>

int main()
{
	pid_t pid;
	pid = fork();
	if(pid == 0)
	{
		for(;;)
		{
			printf("in child : pid = %d\n",getpid());
			sleep(1);
		}
	}
	else if(pid > 0)
	{
		for(int i=0;i<5;i++)
		{
			printf("in parent : pid = %d\n",getpid());
			sleep(1);
		}
	}
	else
		printf("error\n");

	return 0;
}
