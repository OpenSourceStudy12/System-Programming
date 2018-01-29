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
	int listenfd, clientfd, ret, port, nfound, maxfd, account = 0;
	struct sockaddr_in serveraddr, clientaddr;
	int clientaddrlen = sizeof(clientaddr);
	char buff[200], ip[20], cmd[50];
	fd_set set, rset;
	login_t log;
	regist_t reg;
	package_t pack;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	assert(listenfd >= 0);

	setNonBlock(listenfd);
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(7000);
	inet_pton(AF_INET, "127.0.0.1", &serveraddr.sin_addr.s_addr);

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
			else if (!strcmp(cmd, "account")) 
			{
				printf("当前连接：%d\n", account);
				continue;
			}
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
				++account;

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
				memset(&pack, 0, sizeof(pack));
				getpeername(clientfd, (struct sockaddr*)&clientaddr, &clientaddrlen);
				inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, ip, sizeof(ip));
				port = ntohs(clientaddr.sin_port);

				ret = read(clientfd, &pack, sizeof(pack));
				if (ret == 0)
				{
					printf("客户端[%s: %d]断开连接\n", ip, port);
					--account;
					FD_CLR(clientfd, &set);
					close(clientfd);
					continue;
				}
				else if (ret < 0)
				{
					printf("read 出错\n");
					continue;
				}

				if (pack.type == CLI_LOG_REQ)
				{
					ret = read(clientfd, &log, sizeof(log));
					if (ret == 0)
					{
						printf("客户端[%s: %d]断开连接\n", ip, port);
						--account;
						FD_CLR(clientfd, &set);
						close(clientfd);
						continue;
					}
					else if (ret < 0)
					{
						printf("read 出错\n");
						continue;
					}

					printf("客户端[%s:%d]发送数据::%s\t%s\n", ip, port, log.account, log.passwd);
					if (strcmp(log.account, "123456") == 0 && strcmp(log.passwd, "123456") == 0)
					{
						pack.type = CLI_LOG_OK;
					}
					else
						pack.type = CLI_LOG_FL;
				}
				else if (pack.type == CLI_REG_REQ)
				{
					ret = read(clientfd, &reg, sizeof(reg));
					if (ret == 0)
					{
						printf("客户端[%s: %d]断开连接\n", ip, port);
						--account;
						FD_CLR(clientfd, &set);
						close(clientfd);
						continue;
					}
					else if (ret < 0)
					{
						printf("read 出错\n");
						continue;
					}
					printf("客户端[%s:%d]::%s\t%s\t%s\t%s\t%d\n", ip, port, reg.account, reg.passwd, 
							reg.name, reg.tel, reg.age);

					if (strcmp(reg.account, "123456")==0)
					{
						pack.type = CLI_REG_FL;
					}
					else 
						pack.type = CLI_REG_OK;
				}

				ret = write(clientfd, &pack, sizeof(pack));
				if (ret < 0)
				{
					printf("write 出错\n");
					continue;
				}
			}
		}	
	}

	for (int i = 0; i < maxfd+1; ++i)
	{
		if (FD_ISSET(i, &set))
		{
			close(i);
		}
	}
	close(listenfd);
	return 0;
}
