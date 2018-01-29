#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>


int main(int argc,char* argv[])
{
	if(argc < 2)
	{
		printf("缺少参数\n");
		return -1;
	}

	char buff[10];
	int fd,ret;

	fd = open(argv[1],O_RDONLY);
	if(fd < 0)
	{
		perror("open");
		return -1;
	}
	
	while(1)
	{
		ret = read(fd,buff,sizeof(buff));
		if(ret == 0)
			break;
		else if(ret < 0)
		{
			perror("read");
			return -1;
		}
		ret = write(STDOUT_FILENO,buff,ret);
		if(ret < 0)
		{
			perror("write");
			return -1;
		}
	}

	return 0;
}

