#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<assert.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<errno.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<fcntl.h>
#include<sys/epoll.h>

void setNonblock(int fd)
{
	int flag;
	flag = fcntl(fd,F_GETFL);
	flag |= O_NONBLOCK;
	fcntl(fd,F_SETFL,flag);
}

int main()
{
	int epfd,listenfd,clientfd,ret,port,nfound;
	struct sockaddr_in serveraddr,clientaddr;
	int clientaddrlen = sizeof(clientaddr);
	char buff[200],ip[20],cmd[50];
	struct epoll_event events[200],ev;


	listenfd = socket(AF_INET,SOCK_STREAM,0);
	assert(listenfd >= 0);
	setNonblock(listenfd);

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(7000);
	inet_pton(AF_INET,"192.168.1.91",&serveraddr.sin_addr.s_addr);

	ret = bind(listenfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
	assert(ret == 0);

	ret = listen(listenfd,20);
	assert(ret == 0);

	epfd = epoll_create(100);
	assert(epfd >= 0);

	ev.data.fd = listenfd;
	ev.events = EPOLLIN;
	epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev);

	ev.data.fd = STDIN_FILENO;
	ev.events = EPOLLIN;
	epoll_ctl(epfd,EPOLL_CTL_ADD,STDIN_FILENO,&ev);

	for(;;)
	{
		nfound = epoll_wait(epfd,events,200,-1);
		if(nfound < 0)
		{
			perror("epoll_wait");
			continue;
		}

		for(int i=0;i<nfound;i++)
		{
			if(events[i].data.fd == listenfd)
			{
				for(;;)
				{
					clientfd = accept(listenfd,(struct sockaddr*)&clientaddr,&clientaddrlen);
					if(clientfd < 0)
					{
						if(errno == EAGAIN ||errno == EWOULDBLOCK)
							break;
						perror("accept");
						continue;
					}
					port = ntohs(clientaddr.sin_port);
					inet_ntop(AF_INET,&clientaddr.sin_addr.s_addr,ip,sizeof(ip));
					printf("客户端[%s：%d]已连接\n",ip,port);

					setNonblock(clientfd);
					ev.data.fd = clientfd;
					ev.events = EPOLLIN;
					epoll_ctl(epfd,EPOLL_CTL_ADD,clientfd,&ev);

				}
			}

			else if(events[i].data.fd == STDIN_FILENO)
			{
				ret = read(STDIN_FILENO,cmd,sizeof(cmd));
				if(ret < 0)
				{
					perror("read");
					continue;
				}
				cmd[ret-1] = '\0';
				if(!strcmp(cmd,"quit") || !strcmp(cmd,"exit"))
				{
					close(listenfd);
					return 0;
				}
				else
				{
					printf("------------------------------------\n");
					system(cmd);
					printf("------------------------------------\n");
					continue;
				}
			}

			else if(events[i].events & EPOLLIN)
			{
				clientfd = events[i].data.fd;
				getpeername(clientfd,(struct sockaddr*)&clientaddr,&clientaddrlen);
				port = ntohs(clientaddr.sin_port);
				inet_ntop(AF_INET,&clientaddr.sin_addr.s_addr,ip,sizeof(ip));

				bzero(buff,sizeof(buff));
				ret = read(clientfd,buff,sizeof(buff)-1);
				if(ret < 0)
				{
					perror("read");
					continue;
				}
				if(ret == 0)
				{
					fprintf(stdout,"客户端[%s：%d]断开连接\n",ip,port);
					ev.data.fd = clientfd;
					epoll_ctl(epfd,	EPOLL_CTL_DEL,clientfd,&ev);
					continue;
				}
				buff[ret] = '\0';
				printf("收到客户端[%s：%d]的消息::%s\n",ip,port,buff);
#if 0
				sprintf(buff,"this is epoll server");
				ret = write(clientfd,buff,strlen(buff));
				if(ret < 0)
				{
					perror("write");
					continue;
				}
#endif
#if 1
				ev.data.fd = clientfd;
				ev.events = EPOLLOUT;
				epoll_ctl(epfd,EPOLL_CTL_MOD,clientfd,&ev);
#endif
			}
#if 1
			else if(events[i].events & EPOLLOUT)
			{
				clientfd = events[i].data.fd;
				bzero(buff,sizeof(buff));
				sprintf(buff,"this is epoll server");
				ret = write(clientfd,buff,strlen(buff));
				if(ret < 0)
				{
					perror("write");
					continue;
				}
				
				ev.data.fd = clientfd;
				ev.events = EPOLLIN;
				epoll_ctl(epfd,EPOLL_CTL_MOD,clientfd,&ev);
			}	
#endif
		}
	}
}


