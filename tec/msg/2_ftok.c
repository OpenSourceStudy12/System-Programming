#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>

int main()
{
	key_t key;

	key = ftok("msg", 1);
	printf("key = %d\n", key);
	return 0;
}
