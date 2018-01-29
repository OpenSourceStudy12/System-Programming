#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<stdlib.h>

int main()
{
	pid_t pid;
	pid = fork();
	int ret;
	char* name[5]={"file1","file2","file3","file4","file5"};

	if(pid == 0)
	{
		int fd;
		for(int i=0;i<5;i++)
		{
			ret = creat(name[i],0640);
			if(ret < 0)
			{
				perror("creat");
				return -1;
			}
			//sleep(1);
		}
	}
	else if(pid > 0)
	{
		wait(NULL);
		for(int i=0;i<5;i++)
		{
			ret = remove(name[i]);
			if(ret < 0)
			{
				perror("remove");
				return -1;
			}
		}
	}
	else
	{
		perror("fork");
	}
}
