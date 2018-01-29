#include <stdio.h>

int main()
{
	int num = 0x01020304;
	char *p = (char *)&num;

	printf("%d\n", (int)*p);


	return 0;
}
