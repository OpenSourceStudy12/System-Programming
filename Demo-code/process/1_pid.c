#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
	printf("pid = %d\n", getpid());
	printf("ppid = %d\n", getppid());

	return 0;
}
