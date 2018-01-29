#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

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

	fd = open("io", O_RDWR | O_TRUNC | O_CREAT,0640);
	if (fd < 0)
	{
		perror("open");
		return -1;
	}

	for (int i = 0; i < 2; i++)
	{
		printf("please input a struct msg: ");
		scanf("%d %s %c", &stu.id, stu.name, &stu.sex);

		ret = write(fd, &stu, sizeof(stu));
		if (ret < 0)
		{
			fprintf(stderr, "write fail\n");
			return -1;
		}
	}

	printf("input success\n");
	return 0;
}
