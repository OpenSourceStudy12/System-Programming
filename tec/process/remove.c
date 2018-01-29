#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	int fd;

	fd = creat("io", 0640);

	sleep(10);
	close(fd);
	remove("io");
	return 0;
}
