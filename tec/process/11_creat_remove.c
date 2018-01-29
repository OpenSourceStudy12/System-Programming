#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
	pid_t pid;
	int ret;

	char *filename[] = {"file0", "file1", "file2", "file3", "file4"};
	pid = fork();
	if (pid == 0)
	{
		for (int i = 0; i < 5; i++)
		{
			ret = creat(filename[i], 0640);
			if (ret < 0)
			{
				perror("creat");
				return -1;
			}
			sleep(1);
		}
	}
	else if (pid > 0)
	{
		wait(NULL);

		for (int i = 0; i < 5; i++)
		{
			ret = remove(filename[i]);
			if (ret < 0)
			{
				perror("remove");
				return -1;
			}
		}
	}
	else
		perror("fork");

	return 0;
}
