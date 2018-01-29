#include <stdio.h>
#include <unistd.h>

int main()
{
	char buff[200];
	int ret;
	//从终端读取数据.
	ret = read(/*STDIN_FILENO*/0, buff, sizeof(buff)-1);
	if (ret < 0)
	{
		printf("读取失败\n");
		return -1;
	}

	buff[ret] = '\0';
	printf("%s\n", buff);
	printf("STDIN_FILENO = %d  STDOUT_FILENO = %d\n", STDIN_FILENO, STDOUT_FILENO);
	return 0;
}
