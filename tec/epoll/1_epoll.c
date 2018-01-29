#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <netinet/in.h>

void setNonBlock(int fd)
{
	int flags;
	flags = fcntl(fd, F_GETFL);
	flags |= O_NONBLOCK;
	fcntl(fd, F_SETFL, flags);
	return;
}

int main()
{
	int listenfd, clientfd, epollfd, ret, port, nfound;
	struct sockaddr_in serveraddr, clientaddr;
	int clientaddrlen = sizeof(clientaddr);
	char ip[20], buff[200], cmd[50];
	struct epoll_event events[200], ev;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	assert(listenfd >= 0);

	setNonBlock(listenfd);
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(7000);
	//inet_pton(AF_INET, "192.168.1.211", &serveraddr.sin_addr.s_addr);
	//inet_aton
	//inet_ntoa
	serveraddr.sin_addr.s_addr = inet_addr("192.168.1.211");

	ret = bind(listenfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	assert(ret == 0);

	ret = listen(listenfd, 20);
	assert(ret == 0);

	epollfd = epoll_create(1000);
	assert(epollfd >= 0);

	ev.data.fd = listenfd;	//FD_SET(listenfd, &set);
	ev.events = EPOLLIN;
	epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev);

	for (;;)
	{
		nfound = epoll_wait(epollfd, events, 200, -1);
		if (nfound < 0)
		{
			perror("epoll_wait");
			continue;
		}

		for (int i = 0; i < nfound; i++)
		{
			if (events[i].data.fd == listenfd)
			{
				for (;;)
				{
					clientfd = accept(listenfd, (struct sockaddr*)&clientaddr, &clientaddrlen);
					if (clientfd < 0)
					{
						if (errno == EAGAIN || errno == EWOULDBLOCK)
						{
							break;
						}
						perror("accept");
						return -1;
					}
					port = ntohs(clientaddr.sin_port);
					inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, ip, sizeof(ip));
					printf("客户端[%s:%d]建立连接\n", ip, port);

					ev.data.fd = clientfd;
					ev.events = EPOLLIN;
					epoll_ctl(epollfd, EPOLL_CTL_ADD, clientfd, &ev);
				}

			}
			else if (events[i].events & EPOLLIN)
			{
				clientfd = events[i].data.fd;
				getpeername(clientfd, (struct sockaddr*)&clientaddr, &clientaddrlen);
				port = ntohs(clientaddr.sin_port);
				inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, ip, sizeof(ip));

				bzero(buff, sizeof(buff));
				ret = read(clientfd, buff, sizeof(buff)-1);
				if (ret < 0)
				{
					perror("read");
					continue;
				}
				else if (ret == 0)
				{
					printf("客户端[%s:%d]断开连接\n", ip, port);
					close(clientfd);
					ev.data.fd = clientfd;
					ev.events = EPOLLIN;
					epoll_ctl(epollfd, EPOLL_CTL_DEL, clientfd, &ev);
					continue;
				}
				buff[ret] = '\0';
				printf("客户端[%s:%d]发送数据::%s\n", ip, port, buff);

				bzero(buff, sizeof(buff));
				sprintf(buff, "this is a epoll server");
				ret = write(clientfd, buff, strlen(buff));
				if (ret < 0)
				{
					perror("write");
					continue;
				}
			}
		}
	}

	close(epollfd);
	close(listenfd);
	return 0;
}
