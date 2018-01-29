#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
	int ret;

	ret = access("io", F_OK);
	if (ret == 0)
	{
		printf("文件存在\n");
	}
	else
	{
		printf("文件不存在\n");
		return -1;
	}

	ret = access("io", R_OK);
	if (ret == 0)
		printf("文件可读\n");
	else 
		printf("文件不可读\n");

	ret = access("io", X_OK);
	if (ret == 0)
		printf("文件可执行\n");
	else 
		printf("文件不可执行\n");

	return 0;
}
