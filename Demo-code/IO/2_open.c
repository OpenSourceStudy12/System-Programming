#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int main()
{
	int fd;
	//如果不设置， 默认的文件掩码为0002
	umask(0);
//	fd = open("io", O_WRONLY | O_CREAT, 0774);
	//如果将O_CREAT 和 O_EXCL联合使用， 如果文件存在则返回错误。
	fd = open("io", O_WRONLY | O_CREAT | O_EXCL, 0777);
	if (fd < 0)
	{
		printf("打开文件失败\n");
		return -1;
	}
	printf("打开文件成功\n");
	printf("fd = %d\n", fd);

	return 0;
}
