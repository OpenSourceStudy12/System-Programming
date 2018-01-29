#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	int fd;
	fd = open("file", O_RDONLY);
	if (fd < 0)
	{
		fprintf(stderr, "open fail\n");
		return -1;
	}

	int fd2;
	//umask(0);
	fd2 = open("text", O_RDONLY | O_CREAT, 0777);
	if (fd2 < 0)
	{
		fprintf(stderr, "open fail\n");
		return -1;
	}
	printf("fd = %d\n", fd);
	printf("fd2 = %d\n", fd2);
	return 0;
}
