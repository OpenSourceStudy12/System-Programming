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
	struct stu s;

	fd = open("out", O_RDONLY);
	if (fd < 0)
	{
		printf("打开文件失败\n");
		return -1;
	}

	while (1)
	{
		ret = read(fd, &s, sizeof(s));
		if (ret < 0)
		{
			printf("读取文件失败\n");
			return -1;
		}
		else if (ret == 0)
			break;

		printf("\t%d\t%s\t%s\n", s.id, s.name, s.tel);
	}

	return 0;
}
