#include"epoll.h"

void setNonBlock(int fd)
{
	int flags;
	flags=fcntl(fd,F_GETFL);
	flags=flags|O_NONBLOCK;
	fcntl(fd,F_SETFL,flags);
	return;
}


int main()
{
	int listenfd,clientfd,ret,epollfd,nfound,port;
	struct sockaddr_in serveraddr,clientaddr;
	int clientaddrlen=sizeof(clientaddr);
	struct epoll_event ev,events[200];
	char cmd[50],ip[20],buff[200];
	pthread_t pth;

	//创建套接字
	listenfd=socket(AF_INET,SOCK_STREAM,0);
	assert(listenfd>=0);

	//设置服务器IP地址及端口
	setNonBlock(listenfd);//设置非阻塞
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(7000);
	inet_pton(AF_INET,"192.168.1.128",&serveraddr.sin_addr.s_addr);
	
	//绑定服务器IP及端口
	ret=bind(listenfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
	assert(ret==0);

	//监听套接字
	ret=listen(listenfd,20);
	assert(ret==0);

	//创建句柄
	epollfd=epoll_create(1000);
	assert(epollfd>=0);

	//将listenfd注册到监听队列中
	ev.data.fd=listenfd;
	ev.events=EPOLLIN|EPOLLET;
	epoll_ctl(epollfd,EPOLL_CTL_ADD,listenfd,&ev);

	//将STDIN_FILENO注册到监听队列中
	ev.data.fd=STDIN_FILENO;
	ev.events=EPOLLIN|EPOLLET;
	epoll_ctl(epollfd,EPOLL_CTL_ADD,STDIN_FILENO,&ev);

	ret = pool_init(30);
	if (ret < 0)
	{
		printf("初始化线程池失败\n");
		return -1;
	}

	for(;;)
	{
		//等待事件发生
		nfound=epoll_wait(epollfd,events,100,-1);
		if(nfound<0)
		{
			perror("epoll_wait");
			continue;
		}
		else if(nfound==0)
		{
			printf("超时\n");
			continue;
		}
		for(int i=0;i<nfound;++i)//处理所有事件
		{
			if(events[i].data.fd==STDIN_FILENO)
			{
				ret=read(STDIN_FILENO,cmd,sizeof(cmd)-1);
				if(ret<0)
				{
					perror("read");
					continue;
				}
				cmd[ret-1]=0;
				if(!strcmp(cmd,"exit")||!strcmp(cmd,"quit"))
				{
					close(listenfd);
					close(epollfd);
					return 0;
				}
				else
					system(cmd);
			}
			else if(events[i].data.fd==listenfd)//监听到用户连接到绑定的socket端口
			{
				for(;;)
				{
					clientfd=accept(listenfd,(struct sockaddr*)&clientaddr,&clientaddrlen);//接收请求，建立连接，返回套接字文件描述符,用于描述客户端
					if(clientfd<0)
					{
						if(errno==EAGAIN||errno==EWOULDBLOCK)
							break;
						perror("accept");
						continue;
					}	
					port=ntohs(clientaddr.sin_port);
					inet_ntop(AF_INET,&clientaddr.sin_addr.s_addr,ip,sizeof(ip));
					printf("客户端[%s:%d]连接成功\n",ip,port);

					setNonBlock(clientfd);//设置非阻塞
					ev.data.fd=clientfd;
					ev.events=EPOLLIN|EPOLLET;
					epoll_ctl(epollfd,EPOLL_CTL_ADD,clientfd,&ev);//将clientfd添加到监听队列中
				}
			}
			else if(events[i].events&EPOLLIN)//用户传入数据
			{
				clientfd=events[i].data.fd;//设置(epoll中注册的)用户套接字文件描述符
				
				ret = pool_add_worker(read_thread, &clientfd, sizeof(clientfd));//添加任务到线程池
				if (ret < 0)
				{
					printf("添加实现函数失败\n");
					return -1;
				}

			}
			else if(events[i].events&EPOLLOUT)//服务器发送数据
			{
				clientfd=events[i].data.fd;//设置用户套接字文件描述符
				bzero(buff,sizeof(buff));
				sprintf(buff,"this is a epoll server");

				ret=write(clientfd,buff,strlen(buff));
				if(ret<0)
				{
					perror("write");
					continue;
				}
				
				ev.data.fd=clientfd;
				ev.events=EPOLLIN|EPOLLET;
				epoll_ctl(epollfd,EPOLL_CTL_MOD,clientfd,&ev);
			}
		}
	}
	ret = pool_destroy();
	if (ret < 0)
	{
		printf("销毁线程池失败\n");
		return -1;
	}

	close(listenfd);
	close(epollfd);
	return 0;
}
