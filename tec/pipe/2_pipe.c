#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

void sig_handle(int num)
{
	printf("recv a sig: %d\n", num);
}

int main()
{
	pid_t pid;
	int ret, fd[2];
	char buff[200];

	ret = pipe(fd);	//fd[0]--->读    fd[1]---->写
	if (ret < 0)
	{
		perror("pipe");
		return -1;
	}

	close(fd[0]);
	pid  = fork();
	if (pid < 0)
	{
		perror("fork");
		return -1;
	}
	else if (pid == 0)
	{
	//	close(fd[0]);

		if (signal(SIGPIPE, sig_handle) == SIG_ERR)
		{
			fprintf(stderr, "注册信号失败\n");
			return -1;
		}

		while (1)
		{
			memset(buff, 0, sizeof(buff));
			ret = read(STDIN_FILENO, buff, sizeof(buff));
			if (ret < 0)
			{
				perror("read");
				return -1;
			}
			buff[ret-1] = 0;

			if (!strcmp(buff, "exit"))
				break;
			ret = write(fd[1], buff, strlen(buff));
			if (ret < 0)
			{
				if (errno == EPIPE)
				{
					printf("errno = EPIPE\n");
				}
				perror("write");
				return -1;
			}
	//		if (!strcmp(buff, "exit"))
	//			break;
		}
		close(fd[1]);
		exit(0);
	}
	else 
	{
		/*
		close(fd[1]);

		while (1)
		{
			bzero(buff, sizeof(buff));
			ret = read(fd[0], buff, sizeof(buff)-1);
			if (ret < 0)
			{
				perror("read");
				return -1;
			}
			else if (ret == 0)	//当所有写端都关闭时， 读完数据后，read返回0
			{					//-------------------|-->文件结束的标志
				printf("写端全部关闭， 数据以读完\n");
				break;
			}

			buff[ret] = 0;
			if (!strcmp(buff, "exit"))
				break;

			printf("in parent: %s\n", buff);
		}
		*/
		wait(NULL);
	//	close(fd[0]);
		exit(0);
	}
}
