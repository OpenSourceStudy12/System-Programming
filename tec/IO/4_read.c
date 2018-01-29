#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	int fd;
	int ret;
	char buff[200];

	fd = open("file", O_RDONLY);
	if (fd < 0)
	{
		perror("file");
		return -1;
	}

	ret = read(fd, buff, sizeof(buff)-1);
	if (ret < 0)
	{
		perror("read");
		return -1;
	}

	printf("ret = %d\n", ret);
	buff[ret] = 0;
	printf("%s\n", buff);
	return 0;
}
