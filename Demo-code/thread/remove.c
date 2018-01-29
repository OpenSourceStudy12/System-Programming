#include <stdio.h>

int main()
{
	int ret;
	ret = remove("file");
	if (ret < 0)
	{
		printf("删除文件失败\n");
		return -1;
	}

	printf("删除文件成功\n");
	return 0;
}
