#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>


int main()
{
	int fd;

	fd = creat("io", 0640);
	if (fd < 0)
	{
		printf("创建文件失败\n");
		return -1;
	}
	
	printf("创建文件成功 fd = %d\n", fd);
	return 0;
}
