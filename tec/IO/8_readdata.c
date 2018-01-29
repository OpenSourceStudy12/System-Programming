#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

struct student
{
	int id;
	char name[20];
	char sex;
};

int main()
{
	int fd, ret;
	struct student stu;

	fd = open("io", O_RDONLY);
	if (fd < 0)
	{
		perror("io");
		return -1;
	}
/*
	for (int i = 0; i < 5; i++)
	{
		ret = read(fd, &stu, sizeof(stu));
		if (ret < 0)
		{
			perror("read");
			return -1;
		}

		printf("%d\t%s\t%c\n", stu.id, stu.name, stu.sex);
	}
*/
	for (;;)
	{
		ret = read(fd, &stu, sizeof(stu));
		if (ret < 0)
		{
			perror("read");
			return -1;
		}
		else if (ret == 0)
			break;

		printf("%d\t%s\t%c\n", stu.id, stu.name, stu.sex);
	}

	return 0;
}
