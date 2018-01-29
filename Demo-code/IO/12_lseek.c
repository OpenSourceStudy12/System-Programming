#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	int fd, ret;
	char buf[8];

	fd = open("io", O_RDONLY);
	if (fd < 0)
	{
		printf("打开文件失败\n");
		return -1;
	}

	//SEEK_SET 相对文件起始位置进行偏移， 正数向后偏移， 负数向前偏移
	//SEEK_CUR
	//SEEK_END
	//返回值为偏移过后的文件描述符距离文件起始的位置
//	lseek(fd, 8, SEEK_SET);
	ret = lseek(fd, -8, SEEK_END);
	printf("ret = %d\n", ret);
	ret = read(fd, buf, sizeof(buf));
	if (ret < 0)
	{
		printf("读取文件失败\n");
		return -1;
	}

	ret = write(STDOUT_FILENO, buf, ret);
	if (ret < 0)
	{
		printf("write 出错\n");
		return -1;
	}
/*
	lseek(fd, -8, SEEK_CUR);

	ret = read(fd, buf, sizeof(buf));
	if (ret < 0)
	{
		printf("读取文件失败\n");
		return -1;
	}
	ret = write(STDOUT_FILENO, buf, ret);
	if (ret < 0)
	{
		printf("write 出错\n");
		return -1;
	}
*/
	return 0;
}
