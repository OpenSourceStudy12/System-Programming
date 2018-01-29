#include <stdio.h>

void stack()
{
	char ch[1024*1024];
	static int i = 1;
	printf("i = %d\n", i++);
	stack();
}
int main()
{
	stack();

}
