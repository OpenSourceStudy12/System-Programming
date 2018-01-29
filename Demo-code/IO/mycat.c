#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("参数有误\n");
		return -1;
	}

	int fd, ret;
	char buff[200];

	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
	{
		printf("打开文件失败\n");
		return -1;
	}

	while (1)
	{
		ret = read(fd, buff, sizeof(buff));
		if (ret < 0)
		{
			printf("读取文件失败\n");
			return -1;
		}
		else if (ret == 0)
			break;

		ret = write(STDOUT_FILENO, buff, ret);
		if (ret < 0)
		{
			printf("写入终端失败\n");
			return -1;
		}
	}
	printf("\n");

	close(fd);
	return 0;
}
