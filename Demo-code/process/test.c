#include <stdio.h>
#include <unistd.h>

int main()
{
	printf("hello world\n");
	printf("in test pid = %d\n", getpid());
	return 0;
}
