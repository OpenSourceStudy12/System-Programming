#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <netdb.h>

int main()
{
	int listenfd, clientfd, ret, port, maxfd;
	struct sockaddr_in serveraddr, clientaddr;
	char buff[200], ip[20];
	fd_set set, rset;
	int clientaddrlen = sizeof(clientaddr);

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	assert(listenfd >= 0);

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(7000);
	//inet_pton(AF_INET, "192.168.1.211", &serveraddr.sin_addr.s_addr);
	serveraddr.sin_addr.s_addr = INADDR_ANY;

	ret = bind(listenfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	assert(ret == 0);

	ret = listen(listenfd, 20);
	assert(ret == 0);

	FD_ZERO(&set);
	FD_SET(listenfd, &set);
	maxfd = listenfd;

	for (;;)
	{
		rset = set;
		ret = select(maxfd+1, &rset/*可读*/, NULL/*可写*/, NULL/*异常*/, NULL/*超时时间*/);
		if (ret < 0)
		{
			printf("select出错\n");
			return -1;
		}

		if (FD_ISSET(listenfd, &rset))
		{
			clientfd = accept(listenfd, (struct sockaddr*)&clientaddr, &clientaddrlen);
			if (clientfd < 0)
			{
				printf("accept failed\n");
				continue;
			}
			inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, ip, sizeof(ip));
			port = ntohs(clientaddr.sin_port);
			printf("客户端[%s: %d] 已成功连接\n", ip, port);

			FD_SET(clientfd, &set);
			if (maxfd < clientfd)
				maxfd = clientfd;

			continue;
		}

		for (int i = 0; i <= maxfd; i++)
		{
			if (FD_ISSET(i, &rset))
			{
				clientfd = i;
				memset(buff, 0, sizeof(buff));
				getpeername(clientfd, (struct sockaddr*)&clientaddr, &clientaddrlen);
				inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, ip, sizeof(ip));
				port = ntohs(clientaddr.sin_port);
				
				ret = read(clientfd, buff, sizeof(buff)-1);
				if (ret == 0)
				{
					printf("客户端[%s: %d]断开连接\n", ip, port);
					FD_CLR(clientfd, &set);
					continue;
				}
				else if (ret < 0)
				{
					printf("read 出错\n");
					continue;
				}
				buff[ret] = '\0';
				printf("收到客户端[%s: %d]消息::%s\n", ip, port, buff);

				memset(buff, 0, sizeof(buff));
				sprintf(buff, "this is select");
				ret = write(clientfd, buff, strlen(buff));
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
