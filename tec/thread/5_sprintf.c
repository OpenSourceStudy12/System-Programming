#include <stdio.h>
#include <string.h>

int main()
{
	char *str = "www.baidu.com";
	char s[50];
	int num = 10;
	float fl = 3.14;

	sprintf(s, "%s %d %.2f", str, num, fl);
	printf("s = %s\n", s);

	return 0;
}
