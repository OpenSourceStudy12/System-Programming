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
#include "pool.h"

#define 	CLI_LOG			10
#define 	CLI_LOG_OK		11
#define 	CLI_LOG_FL		12
#define		CLI_REG			20
typedef struct 
{
	int type;
	char account[20];
	char passwd[20];
}INFO;

void *client_thread(void *argv)
{
	int clientfd, port;
	char buff[200], ip[20];
	int ret;
	struct sockaddr_in clientaddr;
	INFO info;

	pthread_detach(pthread_self());
	memcpy(&clientfd, argv, sizeof(clientfd));
	memcpy(&clientaddr, argv+sizeof(clientfd), sizeof(clientaddr));
	inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, ip, sizeof(ip));
	port = ntohs(clientaddr.sin_port);
	for (;;)
	{
		memset(&info, 0, sizeof(info));
		ret = read(clientfd, &info, sizeof(info));
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
		printf("recv a msg[%s: %d]: account:%s, passwd:%s\n", ip, port, info.account, info.passwd);

		if (info.type == CLI_LOG)
		{
			if (strcmp(info.account, "shanghai") == 0 && strcmp(info.passwd, "123") == 0)
			{
				info.type = CLI_LOG_OK;
			}
			else
			{
				info.type = CLI_LOG_FL;
			}
		}

		ret = write(clientfd, &info, sizeof(info));
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

	ret = pool_init(30);
	if (ret < 0)
	{
		printf("初始化线程池失败\n");
		return -1;
	}

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
		ret = pool_add_worker(client_thread, buff, sizeof(buff));
		if (ret < 0)
		{
			printf("添加实现函数失败\n");
			return -1;
		}

	}

	ret = pool_destroy();
	if (ret < 0)
	{
		printf("销毁线程池失败\n");
		return -1;
	}

	close(listenfd);
	return 0;
}
