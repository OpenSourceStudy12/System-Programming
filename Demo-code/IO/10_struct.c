#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

struct stu
{
	int id;
	char name[10];
	char tel[10];
};

int main()
{
	int fd, ret;
	struct stu s[] = {{101, "zhang", "1234567"}, {102, "wang", "7654321"}};

	fd = open("out", O_WRONLY | O_TRUNC);
	if (fd < 0)
	{
		printf("打开文件失败\n");
		return -1;
	}

	ret = write(fd, s, sizeof(s));
	if (ret < 0)
	{
		printf("写入文件失败\n");
		return -1;
	}

	return 0;
}
