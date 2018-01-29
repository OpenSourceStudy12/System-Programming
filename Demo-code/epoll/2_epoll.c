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

void setNonBlock(int fd)
{
	int flags;
	flags = fcntl(fd, F_GETFL);
	flags = flags | O_NONBLOCK;
	fcntl(fd, F_SETFL, flags);
	return;
}

int main()
{
	int listenfd, clientfd, epollfd, ret, port, nfound;
	struct sockaddr_in serveraddr, clientaddr;
	int clientaddrlen = sizeof(clientaddr);
	char buff[200], ip[20], cmd[50];
	struct epoll_event ev, events[200];

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

	epollfd = epoll_create(1000);
	assert(epollfd >= 0);

	ev.data.fd = listenfd;
	ev.events = EPOLLIN | EPOLLET;
	epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev);

	ev.data.fd = STDIN_FILENO;
	ev.events = EPOLLIN | EPOLLET;
	epoll_ctl(epollfd, EPOLL_CTL_ADD, STDIN_FILENO, &ev);

	for (;;)
	{
		nfound = epoll_wait(epollfd, events, 100, -1);
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

		for (int i = 0; i < nfound; ++i)
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
			else if (events[i].data.fd == listenfd)
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
						continue;
					}
					port = ntohs(clientaddr.sin_port);
					inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, ip, sizeof(ip));
					printf("客户端[%s:%d]连接成功\n", ip, port);

					setNonBlock(clientfd);
					ev.data.fd = clientfd;
					ev.events = EPOLLIN | EPOLLET;
					epoll_ctl(epollfd, EPOLL_CTL_ADD, clientfd, &ev);
				}
			}
			else if (events[i].events & EPOLLIN)
			{
				clientfd = events[i].data.fd;
				memset(buff, 0, sizeof(buff));
				getpeername(clientfd, (struct sockaddr*)&clientaddr, &clientaddrlen);
				port = ntohs(clientaddr.sin_port);
				inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, ip, sizeof(ip));

				ret = read(clientfd, buff, sizeof(buff)-1);
				if (ret == 0)
				{
					printf("客户端[%s:%d]断开连接\n", ip, port);
					ev.data.fd = clientfd;
					ev.events = EPOLLIN | EPOLLET;
					epoll_ctl(epollfd, EPOLL_CTL_DEL, clientfd, &ev);
					close(clientfd);
					continue;
				}
				else if (ret < 0)
				{
					perror("read");
					continue;
				}

				buff[ret] = 0;
				printf("客户端[%s:%d]发送数据:%s\n", ip, port, buff);

				ev.data.fd = clientfd;
				ev.events = EPOLLOUT | EPOLLET;
				epoll_ctl(epollfd, EPOLL_CTL_MOD, clientfd, &ev);
			}
			else if (events[i].events & EPOLLOUT)
			{
				clientfd = events[i].data.fd;
				bzero(buff, sizeof(buff));
				sprintf(buff, "this is a epoll server");

				ret = write(clientfd, buff, strlen(buff));
				if (ret < 0)
				{
					perror("write");
					continue;
				}

				ev.data.fd = clientfd;
				ev.events = EPOLLIN | EPOLLET;
				epoll_ctl(epollfd, EPOLL_CTL_MOD, clientfd, &ev);
			}
		}
	}

	close(listenfd);
	close(epollfd);
	return 0;
}
