#include <stdio.h>

int main()
{
	int num = 10;
	int *p = &num;
	printf("%p\n", p);
	printf("num = %d\n", *p);
	printf("size = %d\n", sizeof(p));
	printf("*num = %d\n", *(int *)num);
}
