#include <stdio.h>
#include <string.h>

int nToten(int n, const char *num)
{
	int ret = 0;
	int len = strlen(num);
	char ch;
	for (int i = 0; i < len; i++)
	{
		ch = num[i];

		if (ch >= '0' && ch <= '9')
		{
			ret = ret*n + (ch - '0');
		}
		else if (ch >= 'a' && ch <= 'z')
		{
			ret = ret*n + (ch - 87);
		}
		else 
			return -1;
	}

	return ret;
}

int main()
{
	int n;
	char num[33];

	printf("请输入一个n进制的数和进制值\n");
	scanf("%d %s", &n, num);

	int ret = nToten(n, num);
	if (ret < 0)
		printf("输入有误\n");
	else 
	{
		printf("ret = %d\n", ret);
	}

	return 0;
}
