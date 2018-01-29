#include <stdio.h>
#include <unistd.h>

extern char **environ;

int main()
{
	for (int i = 0; environ[i] != NULL; i++)
	{
		printf("%s\n", environ[i]);
	}

	printf("in environ.c\n");
	return 0;
}
