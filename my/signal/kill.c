#include<stdio.h>
#include<unistd.h>
#include<signal.h>

int main(int argc,char** argv)
{
	if(argc != 3)
		return -1;

	int pid=0, sig=0;
#if 0	
	for(int i=0;argv[1][i] != '\0';i++)
	{
		pid = pid*10 + argv[1][i]-'0';
	}
	for(int i=0;argv[2][i] != '\0';i++)
	{
		sig = sig*10+argv[2][i]-'0';
	}
#endif

#if 0
	pid = aoti(argv[1]);
	sig = aoti(argv[2]);
#endif

#if 1
	sscanf(argv[1],"%d",&pid);
	sscanf(argv[2],"%d",&sig);
#endif

	printf("pid = %d,sig = %d\n",pid,sig);
	sleep(2);
	kill(pid,sig);
	return 0;

}
