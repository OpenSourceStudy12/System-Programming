#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

int main()
{
	int fd;

	fd = open("file",O_RDONLY | O_CREAT);
	if(fd < 0)
	{
		fprintf(stdout,"open fail\n");
		return -1;
		//perror();
	}
	fprintf(stdout,"fd = %d\n",fd);

	return 0;
}
