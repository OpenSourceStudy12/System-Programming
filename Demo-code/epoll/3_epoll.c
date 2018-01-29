#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <assert.h>
#include <pthread.h>

int epollfd;

void setNonBlock(int fd)
{
	int flags;
	flags = fcntl(fd, F_GETFL);
	flags = flags | O_NONBLOCK;
	fcntl(fd, F_SETFL, flags);
	return;
}

void *read_thread(void *argv)
{
	int clientfd = (int )argv;
	int ret, port;
	struct sockaddr_in serveraddr, clientaddr;
	int clientaddrlen = sizeof(clientaddr);
	char buff[200], ip[20], cmd[50];
	struct epoll_event ev, events[200];
	memset(buff, 0, sizeof(buff));
	getpeername(clientfd, (struct sockaddr*)&clientaddr, &clientaddrlen);
	port = ntohs(clientaddr.sin_port);
	inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, ip, sizeof(ip));
	pthread_detach(pthread_self());

	printf("thread[%lu]\n", pthread_self());
	ret = read(clientfd, buff, sizeof(buff)-1);
	if (ret == 0)
	{
		printf("客户端[%s:%d]断开连接\n", ip, port);
		ev.data.fd = clientfd;
		ev.events = EPOLLIN | EPOLLET;
		epoll_ctl(epollfd, EPOLL_CTL_DEL, clientfd, &ev);
		close(clientfd);
	//	continue;
		return NULL;
	}
	else if (ret < 0)
	{
		perror("read");
	//	continue;
		return NULL;
	}

	buff[ret] = 0;
	printf("客户端[%s:%d]发送数据:%s\n", ip, port, buff);

	ev.data.fd = clientfd;
	ev.events = EPOLLOUT | EPOLLET;
	epoll_ctl(epollfd, EPOLL_CTL_MOD, clientfd, &ev);//修改标识符可写，用于下次循环发送数据

}
int main()
{
	int listenfd, clientfd,/* epollfd,*/ ret, port, nfound;
	struct sockaddr_in serveraddr, clientaddr;
	int clientaddrlen = sizeof(clientaddr);
	char buff[200], ip[20], cmd[50];
	struct epoll_event ev, events[200];//声明变量，ev用于注册事件，events[]用于回传事件
	pthread_t pth;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	assert(listenfd >= 0);

	setNonBlock(listenfd);
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(7000);
	inet_pton(AF_INET, "192.168.1.128", &serveraddr.sin_addr.s_addr);

	ret = bind(listenfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	assert(ret == 0);

	ret = listen(listenfd, 20);
	assert(ret == 0);

	epollfd = epoll_create(1000);//创建句柄
	assert(epollfd >= 0);

	ev.data.fd = listenfd;//设置事件相关文件描述符
	ev.events = EPOLLIN | EPOLLET;//设置事件类型—————文件描述符可读，边缘触发
	epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev);//注册listenfd——————将listenfd添加到(epollfd)监听队列中

	ev.data.fd = STDIN_FILENO;//设置事件相关文件描述符
	ev.events = EPOLLIN | EPOLLET;//设置事件类型————读操作，边缘触发
	epoll_ctl(epollfd, EPOLL_CTL_ADD, STDIN_FILENO, &ev);//将fd注册到epollfd中，用于监听

	for (;;)
	{
		nfound = epoll_wait(epollfd, events, 100, -1);//等待事件产生
		if (nfound < 0)
		{
			perror("epoll_wait");
			continue;
		}
		else if (nfound == 0)
		{
			printf("超时\n");
			continue;
		}

		for (int i = 0; i < nfound; ++i)//处理所有事件
		{
			if (events[i].data.fd == STDIN_FILENO)
			{
				ret = read(STDIN_FILENO, cmd, sizeof(cmd)-1);
				if (ret < 0)
				{
					perror("read");
					continue;
				}
				cmd[ret-1] = 0;

				if (!strcmp(cmd, "exit") || !strcmp(cmd, "quit"))
				{
					close(listenfd);
					close(epollfd);
					return 0;
				}
				else
					system(cmd);
			}
			else if (events[i].data.fd == listenfd)//监听到socket用户连接到绑定的socket端口
			{
				for (;;)
				{
					clientfd = accept(listenfd, (struct sockaddr*)&clientaddr, &clientaddrlen);//接收连接
					if (clientfd < 0)
					{
						if (errno == EAGAIN || errno == EWOULDBLOCK)
						{
							break;
						}
						perror("accept");
						continue;
					}
					port = ntohs(clientaddr.sin_port);
					inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, ip, sizeof(ip));
					printf("客户端[%s:%d]连接成功\n", ip, port);

					setNonBlock(clientfd);//设置为非阻塞
					ev.data.fd = clientfd;//设置事件相关套接字文件描述符
					ev.events = EPOLLIN | EPOLLET;//设置事件类型————读操作，边缘触发
					epoll_ctl(epollfd, EPOLL_CTL_ADD, clientfd, &ev);//将clientfd注册到epoll等待队列中
				}
			}
			else if (events[i].events & EPOLLIN)//收到数据
			{
				clientfd = events[i].data.fd;
				ret = pthread_create(&pth, NULL, read_thread, (void *)clientfd);//进行读入
				if (ret < 0)
				{
					perror("pthread_create");
					continue;
				}

			}
			else if (events[i].events & EPOLLOUT)//发送数据
			{
				clientfd = events[i].data.fd;
				bzero(buff, sizeof(buff));
				sprintf(buff, "this is a epoll server");

				ret = write(clientfd, buff, strlen(buff));//进行写入
				if (ret < 0)
				{
					perror("write");
					continue;
				}

				ev.data.fd = clientfd;//设置相关的文件描述符
				ev.events = EPOLLIN | EPOLLET;//设置事件类型————读操作，边缘触发
				epoll_ctl(epollfd, EPOLL_CTL_MOD, clientfd, &ev);//修改标识符为可读，等待下一个循环接收数据
			}
		}
	}

	close(listenfd);
	close(epollfd);
	return 0;
}
