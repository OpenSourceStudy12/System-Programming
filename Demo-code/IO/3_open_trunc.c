#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>


int main()
{
	int fd;

	//以只读或者读写的方式打开文件，并且包含量O_TRUNC, 则将会清空文件
	fd = open("io", O_RDWR | O_TRUNC);
	if (fd < 0)
	{
		printf("打开文件失败\n");
		return -1;
	}

	printf("打开文件成功\n");
	printf("fd = %d\n", fd);
	return 0;
}
