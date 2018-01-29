#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>

int main()
{
	int ret;
	pid_t pid;
	int fd[2];
	char buff[200];

	ret = pipe(fd);
	if(ret < 0 )
	{
		perror("pipe");
		return -1;
	}

	pid = fork();
	if(pid < 0)
	{
		perror("fork");
		return -1;
	}
	else if(pid == 0)//写
	{
		close(fd[0]);//关闭读
		while(1)
		{
			memset(buff,0,sizeof(buff));
			ret = read(STDIN_FILENO,buff,sizeof(buff));
			if(ret < 0)
			{
				perror("read");
				return -1;
			}

			buff[ret-1] = 0;
			ret = write(fd[1],buff,strlen(buff));
			if(ret < 0)
			{
				perror("write");
				return -1;
			}

			if(!strcmp(buff,"exit"))
					break;
		}
		close(fd[1]);
	}
	else
	{
		close(fd[1]);//关闭写
		while(1)
		{
			memset(buff,0,sizeof(buff));
			ret = read(fd[0],buff,sizeof(buff)-1);
			if(ret < 0 )
			{
				perror("read");
				return -1;
			}
			buff[ret] = 0;
			if(!strcmp(buff,"exit"))
				break;
			printf("%s\n",buff);
		}
		close(fd[0]);
		wait(NULL);
	}
}
