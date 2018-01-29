#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netinet/in.h>

void setNonblock(int fd)
{
	int flags;
	flags = fcntl(fd, F_GETFL);
	flags = flags | O_NONBLOCK;
	fcntl(fd, F_SETFL, flags);
	return;
}

int main()
{
	int listenfd, clientfd, ret, maxfd, port, nfound;
	struct sockaddr_in serveraddr, clientaddr;
	int clientaddrlen = sizeof(clientaddr);
	char buff[200], ip[20], cmd[50];
	fd_set set, rset;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	assert(listenfd >= 0);

	setNonblock(listenfd);//将listenfd 设为非阻塞
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
	if (STDIN_FILENO > listenfd)
		maxfd = STDIN_FILENO;

	for (;;)
	{
		rset = set;
		nfound = select(maxfd+1, & , NULL, NULL, NULL);
		if (nfound < 0)
		{
			perror("select");
			continue;
		}
		
		if (FD_ISSET(STDIN_FILENO, &rset))
		{
			ret = read(STDIN_FILENO, cmd, sizeof(cmd));
			if (ret < 0)
			{
				perror("read");
				continue;
			}

			cmd[ret-1] = 0;
			if (!strcmp(cmd, "exit") || !strcmp(cmd, "quit"))
				break;
			else 
				system("cmd");
			continue;
		}

		if (FD_ISSET(listenfd, &rset))
		{
			for (;;)
			{
				//accept 在此非阻塞 若数据未到达，立即返回 -----》clientfd<0
				clientfd = accept(listenfd, (struct sockaddr*)&clientaddr, &clientaddrlen);
				if (clientfd < 0)
				{
					if (errno == EAGAIN || errno == EWOULDBLOCK)// 有客户端连接但数据未到达
					{
						break;
					}
					perror("accept");//连接读取数据错误
					continue;
				}

				port = ntohs(clientaddr.sin_port);
				inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, ip, sizeof(ip));
				printf("客户端[%s:%d]建立连接\n", ip, port);

				setNonblock(clientfd);
				if (clientfd > maxfd)
					maxfd = clientfd;

				FD_SET(clientfd, &set);
				//建立连接后进行下次循环 -----》accept 非阻塞 clientfd < 0,break  或连接其它客户端
			}
		}

		for (int i = 0; i < maxfd; ++i)
		{
			if (i == listenfd)
				continue;
			else if (i == STDIN_FILENO)
				continue;
			else if (FD_ISSET(i, &rset))
			{
				clientfd = i;
				getpeername(clientfd, (struct sockaddr*)&clientaddr, &clientaddrlen);
				port = ntohs(clientaddr.sin_port);
				inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, ip, sizeof(ip));
			
				bzero(buff, sizeof(buff));
				ret = read(clientfd, buff, sizeof(buff)-1);//在此非阻塞
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
				sprintf(buff, "this is select server");
				ret = write(clientfd, buff, strlen(buff));
				if (ret < 0)
				{
					perror("write");
					continue;
				}
			}
		}
	}

	close(listenfd);
	return 0;
}
