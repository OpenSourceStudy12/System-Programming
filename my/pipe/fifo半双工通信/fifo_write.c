#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<errno.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

int main()
{
	int fd,ret;
	char buff[200];
	ret = mkfifo("myfifo",0640);
	if(ret < 0 && errno != EEXIST)
	{
		perror("fifo");
		return -1;
	}

	fd = open("myfifo",O_WRONLY);//如果没有进程打开读，本进程将阻塞在此
	if(fd < 0)
	{
		perror("open");
		return -1;
	}

	while(1)
	{
		memset(buff,0,sizeof(buff));
		ret = read(STDIN_FILENO,buff,sizeof(buff)-1);
		if(ret < 0)
		{
			perror("read");
			return -1;
		}
		buff[ret-1] = '\0';
		ret = write(fd,buff,strlen(buff));
		if(ret < 0)
		{
			perror("write");
			return -1;
		}

		if(!strcmp(buff,"exit"))
			break;
	}
	close(fd);
	return 0;
}
