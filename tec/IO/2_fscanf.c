#include <stdio.h>

int main()
{
	FILE *fp;
	int ret;
	char buff[200];
	int num;v

	fp = fopen("file", "r");

	ret = fscanf(fp, "%s %d", buff, &num);
	printf("%s num = %d\n", buff, num);
	printf("ret = %d\n", ret);

	return 0;
}
