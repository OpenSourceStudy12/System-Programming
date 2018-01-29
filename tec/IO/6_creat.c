#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
	int fd;

	fd = creat("io", 0640);
	if (fd < 0)
	{
		perror("creat");
		return -1;
	}

	printf("fd = %d\n", fd);
	return 0;
}
