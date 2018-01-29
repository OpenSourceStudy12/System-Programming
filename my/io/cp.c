#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
//复制文件
#define SIZE 10

int main(int argc,char** argv)
{
	if(argc < 3)
	{	
		fprintf(stderr,"cp fail\n");
		return -1;
	}
	int fd_read,fd_write;
	int ret;
	char buff[SIZE];

	fd_read = open(argv[1],O_RDONLY);
	if(fd_read < 0)
	{
		perror("open");
		return -1;
	}

	fd_write = open(argv[2],O_WRONLY | O_TRUNC | O_CREAT,0640);
	if(fd_write < 0)
	{
		perror("open");
		return -1;
	}
	//复制
	while(1)
	{
		ret = read(fd_read,buff,SIZE);
		if(ret < 0)
		{
			perror("read");
			return -1;
		}
		else if(ret == 0)
			break;
		ret = write(fd_write,buff,ret);
		if(ret < 0)
		{
			perror("write");
			return -1;
		}
	}

	printf("cp success\n");
	//write(STDOUT_FILENO,"cp success\n",11);]
	return 0;
}
