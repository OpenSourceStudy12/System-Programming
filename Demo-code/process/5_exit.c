#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

void fun()
{
	printf("in fun\n");
	//立即退出进程。
	exit(0);
}
int main()
{
	int i = 0;

	fun();
	while(1);
	return 0;
}
