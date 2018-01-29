#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	int fd1, fd2, ret;
	char buff[8];

	fd1 = open("io", O_RDONLY);
	if (fd1 < 0)
	{
		printf("打开文件失败\n");
		return -1;
	}

	fd2 = open("out", O_WRONLY | O_CREAT | O_TRUNC, 0640);
	if (fd2 < 0)
	{
		printf("打开文件失败\n");
		return -1;
	}

	while (1)
	{
		ret = read(fd1, buff, sizeof(buff)-1);
		if (ret < 0)
		{
			printf("读取文件失败\n");
			return -1;
		}
		else if (ret == 0)
			break;

		//	ret = write(fd2, buff, sizeof(buff)-1);
		ret = write(fd2, buff, ret);
		if (ret < 0)
		{
			printf("写入文件失败\n");
			return -1;
		}
	}

	close(fd1);
	close(fd2);
	return 0;
}
