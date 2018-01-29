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

	fd = open("myfifo",O_RDONLY);//如果没有进程打开写，本进程将阻塞在此
	if(fd < 0)
	{
		perror("open");
		return -1;
	}

	while(1)
	{
		memset(buff,0,sizeof(buff));
		ret = read(fd,buff,sizeof(buff)-1);
		if(ret < 0)
		{
			perror("read");
			return -1;
		}
		buff[ret] = '\0';

		if(!strcmp(buff,"exit"))
			break;

		printf("%s\n",buff);
		
	}
	close(fd);
	return 0;
}
