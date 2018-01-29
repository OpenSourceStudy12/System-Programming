#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>

int main()
{
	int key;

	key = ftok("./test", 1);

	printf("key = %d\n", key);
}
