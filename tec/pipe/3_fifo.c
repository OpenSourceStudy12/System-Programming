#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{
	int ret;
	int fd;
	char buff[200];

	ret = mkfifo("myfifo", 0640);
	if (ret < 0 && errno != EEXIST)
	{
	/*	if (errno == EEXIST)
		{
			printf("文件已存在\n");
			return 0;
		}*/
		perror("mkfifo");
		return -1;
	}

	printf("创建管道成功\n");

	fd = open("myfifo", O_WRONLY);
	if (fd < 0)
	{
		perror("open");
		return -1;
	}

	while (1)
	{
		memset(buff, 0, sizeof(buff));

		ret = read(STDIN_FILENO, buff, sizeof(buff)-1);
		if (ret < 0)
		{
			perror("read");
			return -1;
		}
		buff[ret-1] = 0;

		ret = write(fd, buff, strlen(buff));
		if (ret < 0)
		{
			perror("write");
			return -1;
		}

		if (!strcmp(buff, "exit"))
			break;
	}
	close(fd);

	return 0;
}
