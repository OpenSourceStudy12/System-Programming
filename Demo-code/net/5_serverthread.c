#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>


void *client_thread(void *argv)
{
	int clientfd, port;
	char buff[200], ip[20];
	int ret;
	struct sockaddr_in clientaddr;

	pthread_detach(pthread_self());
	memcpy(&clientfd, argv, sizeof(clientfd));
	memcpy(&clientaddr, argv+sizeof(clientfd), sizeof(clientaddr));
	inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, ip, sizeof(ip));
	port = ntohs(clientaddr.sin_port);
	for (;;)
	{
		memset(buff, 0, sizeof(buff));
		ret = read(clientfd, buff, sizeof(buff)-1);
		if (ret == 0)
		{
			printf("客户端[%s: %d]断开连接\n", ip, port);
			break;
		}
		else if (ret < 0)
		{
			printf("读取数据出错\n");
			return NULL;
		}
		printf("recv a msg[%s: %d]: %s\n", ip, port, buff);

		memset(buff, 0, sizeof(buff));
		sprintf(buff, "this is thread");
		ret = write(clientfd, buff, strlen(buff));
		if (ret < 0)
		{
			printf("发送数据失败\n");
			return NULL;
		}
	}

	close(clientfd);
	return NULL;
}

int main()
{
	int listenfd, clientfd, ret;
	struct sockaddr_in serveraddr, clientaddr;
	int clientaddrlen = sizeof(clientaddr);
	char buff[200], ip[20];
	int port;
	pthread_t pth;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	assert(listenfd >= 0);

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(7000);
	inet_pton(AF_INET, "192.168.1.128", &serveraddr.sin_addr.s_addr);
//	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

	ret = bind(listenfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	assert(ret == 0);

	ret = listen(listenfd, 20);
	assert(ret == 0);

	for (;;)
	{
		clientfd = accept(listenfd, (struct sockaddr*)&clientaddr, &clientaddrlen);
		if (clientfd < 0)
		{
			printf("accept failed\n");
			return -1;
		}
		inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, ip, sizeof(ip));
		port = ntohs(clientaddr.sin_port);
		printf("客户端[%s: %d]连接成功\n", ip, port);

		memcpy(buff, &clientfd, sizeof(clientfd));
		memcpy(buff+sizeof(clientfd), &clientaddr, sizeof(clientaddr));
		ret = pthread_create(&pth, NULL, client_thread, buff);
		assert(ret == 0);
	}

	close(listenfd);
	return 0;
}
