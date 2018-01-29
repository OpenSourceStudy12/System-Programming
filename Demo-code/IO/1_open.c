#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int main()
{
	int fd;

//	fd = open("io", O_RDONLY);
	fd = open("io", O_WRONLY);
	if (fd < 0)
	{
		printf("open fail\n");
	}
	else 
	{
		printf("success\n");
	}

	return 0;
}
