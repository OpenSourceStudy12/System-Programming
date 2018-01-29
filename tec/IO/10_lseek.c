#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

//SEEK_SET 相对文件开头偏移， 负数向前偏移， 整数向后偏移
//SEEK_CUR 相对文件描述符当前位置
//SEEK_END 相对文件末尾
//lseek 返回值为偏移后的文件描述符距离文件开头的距离
int main()
{
	int fd, ret;
	char buff[200] = "1234567890";

	fd = open("io", O_RDWR | O_CREAT, 0640);
	if (fd < 0)
	{
		perror("open");
		return -1;
	}

//	ret = lseek(fd, 0, SEEK_END);
//	ret = lseek(fd, 5, SEEK_SET);
	ret = lseek(fd, -5, SEEK_END);
	if (ret == -1)
	{
		perror("lseek");
		return -1;
	}
	printf("ret = %d\n", ret);

	ret = read(fd, buff, sizeof(buff));
	if (ret < 0)
	{
		perror("write");
		return -1;
	}
	buff[ret] = 0;
	printf("%s\n", buff);

	printf("success\n");
	return 0;
}
