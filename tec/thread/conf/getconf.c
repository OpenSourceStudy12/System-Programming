#include <stdio.h>
#include <string.h>
#include <unistd.h>

int getconf(char *account)
{
	char line[200];
	char key[20];
	FILE *fp;
	fp = fopen("data.cnf", "r");
	if (fp == NULL)
	{
		perror("fopen");
		return -1;
	}

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
	int ret;

	ret = getconf(account);
	if (ret < 0)
	{
		fprintf(stderr, "read fail\n");
		return -1;
	}

	printf("account = %s\n", account);
	return 0;
}
