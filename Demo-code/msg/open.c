#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

int main()
{
	int fd;
	fd = open("io", O_WRONLY);
	if (fd < 0)
	{
		perror("open");
		return -1;
	}
}
