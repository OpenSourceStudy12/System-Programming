#include <stdio.h>
#include <string.h>

int getconf(char *account)
{
	FILE *fp = fopen("data.cnf", "r");
	char line[200];
	char key[100];

	while (!feof(fp))
	{
		fgets(line, sizeof(line), fp);
		sscanf(line, "%s", key);
		if (key[0] == '#')
			continue;

		if (strcmp(key, "account") == 0)
		{
			sscanf(line, "%*s%*s%s", account);
			return 0;
		}
	}

	return -1;
}

int main()
{
	char account[20];
	int ret = getconf(account);
	if (ret < 0)
	{
		printf("没有找到\n");
		return -1;
	}

	printf("account = %s\n", account);
	return 0;
}
