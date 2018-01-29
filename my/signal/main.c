#include<stdio.h>
#include<unistd.h>

int main()
{
	while(1)
	{
		printf("main is running\n");
		usleep(500*1000);
	}
}
