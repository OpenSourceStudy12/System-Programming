#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <assert.h>

int main()
{
	int listenfd, clientfd, ret, port;
	struct sockaddr_in serveraddr, clientaddr;
	int clientaddrlen = sizeof(clientaddr);
	char buff[200];
	char ip[20];

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	assert(listenfd >= 0);

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(7000);
	inet_pton(AF_INET, "192.168.1.211", &serveraddr.sin_addr.s_addr);	//127.0.0.1本地环回ip

	ret = bind(listenfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	assert(ret == 0);

	ret = listen(listenfd, 20);
	assert(ret == 0);

	for (;;)
	{
		printf("before\n");
		clientfd = accept(listenfd, (struct sockaddr*)&clientaddr, &clientaddrlen);//&sizeof(clientaddr)
		assert(clientfd >= 0);
		printf("after\n");

		port = ntohs(clientaddr.sin_port);
		inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, ip, sizeof(ip));
		printf("客户端[%s:%d]建立连接\n", ip, port);

		while (1)
		{
			memset(buff, 0, sizeof(buff));
			ret = read(clientfd, buff, sizeof(buff)-1);
			if (ret < 0)
			{
				perror("read");
				return -1;
			}
			else if (ret == 0)
			{
				printf("与客户端断开连接\n");
				break;
			}
			buff[ret] = 0;
			printf("recv a msg: %s\n", buff);

			bzero(buff, sizeof(buff));
			sprintf(buff, "this is a server");
			ret = write(clientfd, buff, strlen(buff));
			if (ret < 0)
			{
				perror("write");
				return -1;
			}
		}
	}

	close(clientfd);
	close(listenfd);
	return 0;
}
