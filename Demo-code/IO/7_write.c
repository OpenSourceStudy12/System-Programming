#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() 
{
	int fd, ret;
	char *buff = "123456";
	
//	fd = open("io", O_WRONLY | O_APPEND);
	//如果不加append, 则会从文件开头进行覆盖
	fd = open("io", O_WRONLY);
	if (fd < 0) {
		printf("打开文件失败\n");
		return -1;
	}

	ret = write(fd, buff, 6);
	if (ret < 0) {
		printf("写入文件失败\n");
		return -1;
	}

	return 0;
}
