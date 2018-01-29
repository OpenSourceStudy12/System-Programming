#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <assert.h>

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("too few argument %s: \n", argv[0]);
		return -1;
	}
	int clientfd, listenfd, ret;
	struct sockaddr_in serveraddr, clientaddr;
	int clientaddrlen = sizeof(clientaddr);
	char buff[200];
	char ip[20];
	int port;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd < 0)
	{
		perror("socket");
		return -1;
	}

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(atoi(argv[1]));
	inet_pton(AF_INET, "192.168.1.211", &serveraddr.sin_addr.s_addr);

	ret = bind(listenfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (ret < 0)
	{
		perror("bind");
		return -1;
	}

	ret = listen(listenfd, 20);
	if (ret < 0)
	{
		perror("listen");
		return -1;
	}

	clientfd = accept(listenfd, (struct sockaddr*)&clientaddr, &clientaddrlen);
	if (clientfd < 0)
	{
		perror("accept");
		return -1;
	}
	inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, ip, sizeof(ip));
	port = ntohs(clientaddr.sin_port);
	printf("客户端[%s: %d]建立连接\n", ip, port);

	ret = read(clientfd, buff, sizeof(buff)-1);
	if (ret == 0)
	{
		printf("客户端[%s: %d]断开连接\n", ip, port);
		return -1;
	}
	buff[ret] = 0;
	printf("recv a msg[%s: %d] : %s\n", ip, port, buff);

	memset(buff, 0, sizeof(buff));
	sprintf(buff, "i recv");
	ret = write(clientfd, buff, strlen(buff));
	if (ret < 0)
	{
		printf("发送失败\n");
		return -1;
	}

	close(clientfd);
	close(listenfd);
	return 0;
}
