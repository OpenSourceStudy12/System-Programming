#include <stdio.h>

int main()
{
	int num = 10;
	FILE *fp = fopen("file", "rb+");
	int ret = fprintf(fp, "%d",23445);
	if (ret < 0)
	{
		printf("output fail\n");
		return -1;
	}
	printf("ret = %d\n", ret);

	fprintf(stderr, "1234567890\n");

	fclose(fp);
	return 0;
}
