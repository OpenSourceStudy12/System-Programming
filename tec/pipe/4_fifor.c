#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

int main()
{
	int ret, fd;
	char buff[200];

	ret = mkfifo("myfifo", 0640);
	if (ret < 0 && errno != EEXIST)
	{
		perror("mkfifo");
		return -1;
	}

	fd = open("myfifo", O_RDONLY);
	if (fd < 0)
	{
		perror("open");
		return -1;
	}

	while (1)
	{
		memset(buff, 0, sizeof(buff));

		ret = read(fd, buff, sizeof(buff)-1);
		if (ret < 0)
		{
			perror("read");
			return -1;
		}
		else if (ret == 0)
			break;

		buff[ret] = 0;
		printf("recv a msg: %s\n", buff);
	}
	close(fd);
	return 0;
}
