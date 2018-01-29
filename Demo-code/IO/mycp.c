#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		printf("参数有误\n");
		return -1;
	}

	int fd_r, fd_w, ret;
	char opt;
	char buff[200];

	fd_r = open(argv[1], O_RDONLY);
	if (fd_r < 0)
	{
		printf("打开文件失败\n");
		return -1;
	}
	fd_w = open(argv[2], O_WRONLY | O_CREAT | O_EXCL, 0640);
	if (fd_w < 0)
	{
		printf("文件[%s]已存在， 是否覆盖?(Y/N)\n", argv[2]);
		scanf("%c", &opt);
		if (opt == 'N')
			return -1;
		else if (opt == 'Y')
			fd_w = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0640);
		else 
		{
			printf("输入有误\n");
			return -1;
		}
	}

	while (1)
	{
		ret = read(fd_r, buff, sizeof(buff));
		if (ret < 0)
		{
			printf("读取文件失败\n");
			return -1;
		}
		else if (ret == 0)
			break;

		ret = write(fd_w, buff, ret);
		if (ret < 0)
		{
			printf("写入终端失败\n");
			return -1;
		}
	}
	printf("复制成功\n");

	close(fd_r);
	close(fd_w);
	return 0;
}
