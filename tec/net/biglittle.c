#include <stdio.h>

int main()
{
	int a = 0x01020304;
	char *p = &a;
	if (*p == 0x04)
	{
		printf("little\n");
	}
	else
		printf("big\n");

	printf("*p = %d\n", (int)*p);
}
