#include <stdio.h>
#include <string.h>

int nToten(int number, char *str)
{
	int len = strlen(str);
	int sum = 0;

	for (int i = 0; i < len; i++)
	{
		if (str[i] <= '9' && str[i] >= '0' )
		{
			if ((str[i] - '0') >= number)
				return -1;

			sum = (sum + str[i] - '0') * number;
		}
		else if (str[i] >= 'a' && str[i] <= 'z')
		{
			if ((str[i] - 87) >= number)
				return -1;

			sum = (sum + str[i] - 87) * number;
		}
		else 
			return -1;
	}

	sum /= number;

	return sum;
}

int main()
{
	int number;
	char str[50];
	int ret;

	printf("请输入n进制:");
	scanf("%d", &number);
	printf("请输入数: ");
	scanf("%s", str);

	ret = nToten(number, str);
	if (ret < 0)
	{
		printf("input error\n");
		return -1;
	}

	printf("ret = %d\n", ret);
	return 0;
}
