#include<stdio.h>
#include<unistd.h>

int main()
{
#if 0
	pid_t pid;
	pid = fork();
	if(pid == 0)
	{
		//子进程
		printf("child process\n");
		printf("pid = %d\n",getpid());
		printf("ppid = %d\n",getppid());
	}	       
	else if(pid > 0)
	{
		//父进程
		printf("parent process pid = %d\n",pid);
		printf("pid = %d\n",getpid());
		printf("ppid = %d\n",getppid());
	}
	else
		printf("error\n");

	return 0;
#endif
#if 0
	printf("6");
	for(int i=0;i<3;i++)
		fork();
#endif
#if 1
/*
创建如下进程：
bash--A-----B
	     |
		 |
		 |--C--F
		 |
		 ---D
*/
	pid_t pid;
	pid = fork();
	if(pid > 0)//A
	{
		pid = fork();
		if(pid > 0)//A
		{
			//pid_t pid;
			pid = fork();
			if(pid > 0)//A
			{
				while(1);
			}
			else if(pid == 0)//C
			{
				//pid_t pid;
				pid = fork();
				if(pid > 0)//C
				{
					while(1);
				}
				else if(pid == 0)//F
				{
					while(1);
				}
			}
		}
		else if(pid == 0)//D
		{
			while(1);
		}
	}
	else if(pid == 0)//B
	{
		while(1);
	}
	
	return 0;
}
