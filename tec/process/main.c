#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	printf("argv[1] = %s\n", argv[1]);
	printf("in main, id = %d\n", getpid());
	return 0;
}
