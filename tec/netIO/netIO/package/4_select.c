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
#include <sys/select.h>
#include <assert.h>
#include "package.h"

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
	int listenfd, clientfd, ret, port, nfound, maxfd;
	struct sockaddr_in serveraddr, clientaddr;
	int clientaddrlen = sizeof(clientaddr);
	char buff[200], ip[20], cmd[50];
	fd_set set, rset;
	package_t head;
	login_t log;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	assert(listenfd >= 0);

	setNonBlock(listenfd);
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(7000);
	inet_pton(AF_INET, "192.168.1.211", &serveraddr.sin_addr.s_addr);

	ret = bind(listenfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	assert(ret == 0);

	ret = listen(listenfd, 20);
	assert(ret == 0);

	FD_ZERO(&set);
	FD_ZERO(&rset);
	FD_SET(listenfd, &set);
	maxfd = listenfd;
	FD_SET(STDIN_FILENO, &set);
	if (maxfd < STDIN_FILENO)
		maxfd = STDIN_FILENO;

	for (;;)
	{
		rset = set;
		nfound = select(maxfd+1, &rset, NULL, NULL, NULL);
		if (nfound < 0)
		{
			perror("select");
			continue;
		}

		if (FD_ISSET(STDIN_FILENO, &rset))
		{
			ret = read(STDIN_FILENO, cmd, sizeof(cmd)-1);
			if (ret < 0)
			{
				perror("read");
				continue;
			}
			cmd[ret-1] = '\0';

			if (!strcmp(cmd, "quit") || !strcmp(cmd, "exit"))
				break;
			else 
			{
				system(cmd);
				continue;
			}
		}

		if (FD_ISSET(listenfd, &rset))
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
				printf("客户端[%s:%d]建立连接\n", ip, port);

				setNonBlock(clientfd);
				FD_SET(clientfd, &set);
				if (maxfd < clientfd)
					maxfd = clientfd;
			}
		}
		for (int i = 0; i < maxfd+1; ++i)
		{
			if (i == listenfd)
				continue;
			if (FD_ISSET(i, &rset))
			{
				clientfd = i;
				memset(buff, 0, sizeof(buff));
				getpeername(clientfd, (struct sockaddr*)&clientaddr, &clientaddrlen);
				inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, ip, sizeof(ip));
				port = ntohs(clientaddr.sin_port);

				ret = read(clientfd, &head, sizeof(head));
				if (ret == 0)
				{
					printf("客户端[%s: %d]断开连接\n", ip, port);
					FD_CLR(clientfd, &set);
					close(clientfd);
					continue;
				}
				else if (ret < 0)
				{
					printf("read 出错\n");
					continue;
				}

				if (head.type == CLI_LOG_REQ)
				{
					ret = read(clientfd, &log, sizeof(log));
					if (ret == 0)
					{
						printf("客户端[%s: %d]断开连接\n", ip, port);
						FD_CLR(clientfd, &set);
						close(clientfd);
						continue;
					}
					else if (ret < 0)
					{
						printf("read 出错\n");
						continue;
					}

					if (!strcmp(log.account, "admin") && !strcmp(log.passwd, "123456"))
						head.type = CLI_LOG_OK;
					else
						head.type = CLI_LOG_FL;
				}
				else if (head.type == CLI_REG_REQ)
				{
				}

				ret = write(clientfd, &head, sizeof(head));
				if (ret < 0)
				{
					printf("write 出错\n");
					continue;
				}
			}
		}	
	}
	close(listenfd);
	return 0;
}
