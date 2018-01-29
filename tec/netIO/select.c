#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main()
{
	int listenfd, clientfd, maxfd, ret, port, nfound;
	struct sockaddr_in serveraddr, clientaddr;
	int clientaddrlen = sizeof(clientaddr);
	char buff[200], ip[20], cmd[20];
	fd_set set, rset;
	struct timeval value;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	assert(listenfd >= 0);

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(7000);
	inet_pton(AF_INET, "192.168.1.91", &serveraddr.sin_addr.s_addr);

	ret = bind(listenfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	assert(ret == 0);

	ret = listen(listenfd, 20);
	assert(ret == 0);

	FD_ZERO(&set);
	FD_SET(listenfd, &set);
	maxfd = listenfd;

	FD_SET(STDIN_FILENO, &set);
	if (maxfd < STDIN_FILENO)
		maxfd = STDIN_FILENO;

	value.tv_sec = 2;
	value.tv_usec = 0;

	for (;;)
	{
		rset = set;
		nfound = select(maxfd+1, &rset/*read*/, NULL/*write*/, NULL/*error*/, &value/*time*/);
		if (nfound < 0)
		{
			perror("select");
			continue;
		}
		else if (nfound == 0)
		{
			value.tv_sec = 2;
			value.tv_usec = 0;
			printf("超时\n");
			continue;
		}

		if (FD_ISSET(STDIN_FILENO, &rset))
		{
			bzero(cmd, sizeof(cmd));
			ret = read(STDIN_FILENO, cmd, sizeof(cmd)-1);
			if (ret < 0)
			{
				perror("read");
				continue;
			}
			cmd[ret-1] = 0;

			if (!strcmp(cmd, "exit") || !strcmp(cmd, "quit"))
				break;
			else
				system(cmd);

			continue;
		}

		if (FD_ISSET(listenfd, &rset))
		{
			clientfd = accept(listenfd, (struct sockaddr*)&clientaddr, &clientaddrlen);
			if (clientfd < 0)
			{
				perror("accept");
				continue;
			}

			port = ntohs(clientaddr.sin_port);
			inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, ip, sizeof(ip));
			printf("客户端[%s:%d]建立连接\n", ip, port);

			FD_SET(clientfd, &set);
			if (clientfd > maxfd)
				maxfd = clientfd;

			continue;
		}

		for (int i = 0; i <= maxfd && nfound; i++)
		{
			if (FD_ISSET(i, &rset))
			{
				nfound--;

				clientfd = i;
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
					FD_CLR(clientfd, &set);
					continue;
				}

				buff[ret] = 0;
				printf("客户端[%s:%d]发送消息::%s\n", ip, port, buff);

				bzero(buff, sizeof(buff));
				sprintf(buff, "this is a select server");
				ret = write(clientfd, buff, strlen(buff));
				if (ret < 0)
				{
					perror("write");
					continue;
				}
			}
		}
	}

	for (int i = 0; i < maxfd; ++i)
	{
		if(FD_ISSET(i, &set))
			close(i);
	}

	close(listenfd);
	return 0;
}
