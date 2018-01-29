#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main()
{
	char buff[200];
	int ret;

//	close(STDIN_FILENO);
	ret = read(/*STDIN_FILENO*/0, buff, sizeof(buff)-1);
	if (ret < 0)
	{
		perror("read");
		return -1;
	}
	buff[ret] = 0;
//	printf();
	ret = write(STDOUT_FILENO, buff, strlen(buff));
	if (ret < 0)
	{
		perror("write");
		return -1;
	}

	return 0;
}
