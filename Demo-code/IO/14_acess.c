#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("参数有误\n");
		return -1;
	}

	int ret;

	ret = access("io", F_OK);
	if (ret == 0)
		printf("文件存在\n");
	else 
		printf("文件不存在\n");

	if (access(argv[1], F_OK) < 0)
	{
		printf("文件%s不存在\n", argv[1]);
		return -1;
	}

	if (access(argv[1], W_OK) < 0)
		printf("不可写\n");
	else
		printf("W_OK\n");

	if (access(argv[1], R_OK) < 0)
		printf("不可读\n");
	else
		printf("R_OK\n");

	return 0;
}
