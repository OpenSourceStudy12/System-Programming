#include "waiter_client.h"

int clientfd;

int main()
{
	int ret;
	struct sockaddr_in serveraddr;
	char ip[20],port[10],buff[200];

	clientfd = socket(AF_INET,SOCK_STREAM,0);
	assert(clientfd >= 0);

	if(signal(SIGALRM,sig_handle) == SIG_ERR)
	{
		perror("signal");
		return -1;
	}

	if(getcnf("ip",ip) < 0)
	{
		fprintf(stdout,"配置文件读取失败\n");
		return -1;
	}
	if(getcnf("port",port) < 0)
	{
		fprintf(stdout,"配置文件读取失败\n");
		return -1;
	}

//	printf("ip = %s,port = %d\n",ip,atoi(port));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(atoi(port));
	inet_pton(AF_INET,ip,&serveraddr.sin_addr.s_addr);

	ret = connect(clientfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
	if(ret != 0)
	{
		fprintf(stdout,"%s  %d\n",__FILE__,__LINE__);
		perror("connect");
		return -1;
	}
	alarm(10);
	main_fun(clientfd);
	close(clientfd);
	return 0;
}
