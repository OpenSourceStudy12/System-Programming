#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
	char cmd[200];
	int ret;

//	while (1)
	for(;;)
	{
		ret = read(STDIN_FILENO, cmd, sizeof(cmd)-1);
		if (ret < 0)
			return -1;

		cmd[ret-1] = '\0';
		if (strcmp(cmd, "quit") == 0 || strcmp(cmd, "exit") == 0)
			break;

		system(cmd);
	}

	return 0;
}
