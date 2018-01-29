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
#include<errno.h>
#include<fcntl.h>

void setNonblock(int fd)
{
	int flag;
	flag = fcntl(fd,F_GETFL);
	flag |= O_NONBLOCK;
	fcntl(fd,F_SETFL,flag);
}


int main(int argc,char **argv)
{
	int listenfd,clientfd,ret,port,nfound,maxfd;
	struct sockaddr_in serveraddr,clientaddr;
	char buff[200],cmd[50],ip[20];
	int clientaddrlen = sizeof(clientaddr);
	fd_set set,rset;

	//创建套接字
	listenfd = socket(AF_INET,SOCK_STREAM,0);
	assert(listenfd >= 0);
	setNonblock(listenfd);//将监听套接字描述符设为非阻塞式

	//设置ip和端口
	if(argv[1] != NULL)
		port = atoi(argv[1]);
	else
		port = 7000;

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);
	inet_pton(AF_INET,"192.168.1.91",&serveraddr.sin_addr.s_addr);

	//绑定ip和端口
	ret = bind(listenfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
	assert(ret == 0);

	//监听
	ret = listen(listenfd,20);
	assert(ret == 0);

	//清零
	FD_ZERO(&set);
	FD_ZERO(&rset);

	FD_SET(listenfd,&set);//添加listenfd
	maxfd = listenfd;

	FD_SET(STDIN_FILENO,&set);//将标准输入加入set


	for(;;)
	{
		rset = set;
		nfound = select(maxfd+1,&rset,NULL,NULL,NULL);
		if(nfound < 0)
		{
			perror("select");
			continue;
		}
		/*
		   else if(nfound == 0)
		   {
		   printf("超时\n");
		   continue;
		   }
		 */
		if(FD_ISSET(STDIN_FILENO,&rset))
		{
			ret = read(STDIN_FILENO,buff,sizeof(cmd));
			if(ret < 0)
			{
				perror("read");
				continue;
			}
			buff[ret-1] = '\0';
			if(!strcmp(cmd,"exit") || !strcmp(cmd,"quit"))
			{
				break;
			}
			else
			{
				system(cmd);
				continue;
			}
		}

		if(FD_ISSET(listenfd,&rset))
		{
			for(;;)
			{
				clientfd = accept(listenfd,(struct sockaddr*)&clientaddr,&clientaddrlen);
				if(clientfd < 0)
				{
					if(errno == EAGAIN || errno == EWOULDBLOCK)
						break;
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
		}

		for(int i=0;i <= maxfd;i++)
		{
			if(i == STDIN_FILENO)
				continue;
			else if(i == listenfd)
				continue;

			else if(FD_ISSET(i,&rset))
			{
				clientfd = i;
				getpeername(clientfd,(struct sockaddr*)&clientaddr,&clientaddrlen);
				port = ntohs(clientaddr.sin_port);
				inet_ntop(AF_INET,&clientaddr.sin_addr.s_addr,ip,sizeof(ip));

				bzero(buff,sizeof(buff));
				ret = read(i,buff,sizeof(buff)-1);
				printf("1234\n");
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

	for(int i=0;i<=maxfd;i++)
	{
		if(FD_ISSET(i,&set) && i!=STDIN_FILENO)
			close(i);
	}
	return 0;
}
