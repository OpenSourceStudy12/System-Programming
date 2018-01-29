#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<assert.h>
#include<sys/select.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<error.h>
#include<fcntl.h>

int main()
{
	int listenfd,clientfd,ret,port,nfound,maxfd;
	struct sockaddr_in serveraddr,clientaddr;
	char buff[200],cmd[50],ip[20];
	int clientaddrlen = sizeof(clientaddr);
	fd_set set,rset;

	//创建套接字
	listenfd = socket(AF_INET,SOCK_STREAM,0);
	assert(listenfd >= 0);

	//设置ip和端口
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(7000);
	inet_pton(AF_INET,"192.168.1.91",&serveraddr.sin_addr.s_addr);

	//绑定ip和端口
	ret = bind(listenfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
	assert(ret == 0);

	//监听
	ret = listen(listenfd,20);
	assert(ret == 0);

	FD_ZERO(&set);
	FD_ZERO(&rset);
	FD_SET(listenfd,&set);
	maxfd = listenfd;

	while(1)
	{
		rset = set;
		nfound = select(maxfd+1,&rset,NULL,NULL,NULL);
		if(nfound < 0)
		{
			perror("select");
			continue;
		}
		else if(nfound == 0)
		{
			printf("超时\n");
			continue;
		}

		if(FD_ISSET(listenfd,&rset))
		{
			clientfd = accept(listenfd,(struct sockaddr*)&clientaddr,&clientaddrlen);
			if(clientfd < 0)
			{
				perror("accept");
				continue;//重新连接客户端
			}

			port = ntohs(clientaddr.sin_port);
			inet_ntop(AF_INET,&clientaddr.sin_addr.s_addr,ip,sizeof(ip));
			printf("客户端[%s:%d]已连接\n",ip,port);
		
			FD_SET(clientfd,&set);
			if(maxfd < clientfd)
				maxfd = clientfd;

			continue;//与其他客户端连接
		}
		
		for(int i=0;i <= maxfd;i++)
		{
			if(FD_ISSET(i,&rset))
			{
				clientfd = i;
				getpeername(clientfd,(struct sockaddr*)&clientaddr,&clientaddrlen);
				port = ntohs(clientaddr.sin_port);
				inet_ntop(AF_INET,&clientaddr.sin_addr.s_addr,ip,sizeof(ip));
			
				bzero(buff,sizeof(buff));
				ret = read(i,buff,sizeof(buff)-1);
				if(ret < 0)
				{
					perror("read");
					continue;
				}
					
				else if(ret == 0)
				{
					printf("客户端[%s:%d]已断开连接\n",ip,port);
					FD_CLR(i,&set);
					continue;
				}
				
				printf("收到客户端[%s:%d]的消息::%s\n",ip,port,buff);

				sprintf(buff,"this is server");
				ret = write(i,buff,strlen(buff));
				if(ret < 0)
				{
					perror("write");
					continue;
				}
			}
		}
	}

}
