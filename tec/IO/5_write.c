#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main()
{
	int fd;
	char buff[200] = "www.163.com.cn";
	int ret;

//	fd = open("file", O_WRONLY | O_CREAT | O_TRUNC, 0640);
	fd = open("file", O_WRONLY);
	if (fd < 0) 
	{
		fprintf(stderr, "file : %s\n", strerror(errno));
		return -1;
	}

	ret = write(fd, buff, strlen(buff));
	if (ret < 0)
	{
		fprintf(stderr, "write fail\n");
		return -1;
	}
	printf("ret = %d\n", ret);

	return 0;
}
