#include<stdio.h>
#include<signal.h>
#include<unistd.h>

int main()
{
	if(signal(SIGTRAP,SIG_IGN) == SIG_ERR)
	{
		perror("signal");
		return -1;
	}

	while(1)
	{
		printf("running\n");
		usleep(500*1000);
	}

	return 0;
}
