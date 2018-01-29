#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<assert.h>
#include<pthread.h>

typedef struct cli
{
	int clientfd;
	struct sockaddr_in clientaddr;
}Cli;

void* thread_handle(void* argv)
{
	Cli cl = *(Cli*)argv;
	int port,ret;
	char ip[20],buff[200];

	//线程分离(结束后，资源交给系统进行回收)
	pthread_detach(pthread_self());

	port = ntohs(cl.clientaddr.sin_port);
	inet_ntop(AF_INET,&cl.clientaddr.sin_addr.s_addr,ip,sizeof(ip));
	printf("客户端[%s：%d]已连接\n",ip,port);
	while(1)
	{
		memset(buff,0,sizeof(buff));
		ret = read(cl.clientfd,buff,sizeof(buff));
		if(ret < 0)
		{
			perror("read");
			printf("no\n");
			continue;
		}
		else if(ret == 0)
		{
			printf("客户端[%s：%d]已断开连接\n",ip,port);
			break;
		}
		printf("receive a msg :%s\n",buff);
	}
	close(cl.clientfd);
	return NULL;
}

int main()
{
	int listenfd,ret;
	struct sockaddr_in serveraddr;
	char buff[200];
	int cliaddr_len = sizeof(struct sockaddr_in);
	Cli cl;
	pthread_t pth;

	//创建套接字
	listenfd = socket(AF_INET,SOCK_STREAM,0);
	assert(listenfd >= 0);

	//设置ip和端口
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = ntohs(7000);
	inet_pton(AF_INET,"192.168.1.91",&serveraddr.sin_addr.s_addr);

	//绑定
	ret = bind(listenfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
	assert(ret == 0);

	//监听
	ret = listen(listenfd,20);
	assert(ret == 0);

	for(;;)
	{
		//等待客户端连接
		cl.clientfd = accept(listenfd,(struct sockaddr*)&cl.clientaddr,&cliaddr_len);
		assert(cl.clientfd >= 0);

		pthread_create(&pth,NULL,thread_handle,&cl);
	}

	//pthread_join(pth,NULL);

}
