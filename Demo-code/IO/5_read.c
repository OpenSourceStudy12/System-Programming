#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	int fd;
	char buff[1024];
	int ret;

	fd = open("io", O_RDONLY);
	if (fd < 0)
	{
		printf("打开文件失败\n");
		return -1;
	}

	//read 会将文件的结束符转换成换行
	ret = read(fd, buff, 1023);
	if (ret < 0)
	{
		printf("读取文件失败\n");
		return -1;
	}

	buff[ret] = '\0';
	printf("%s\n", buff);
	return 0;
}
