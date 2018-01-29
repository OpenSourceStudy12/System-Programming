#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <signal.h>
#include <sys/wait.h>

int clientTask(int clientfd, struct sockaddr_in clientaddr)
{
	int ret;
	char buff[200];
	char ip[20];
	int port;

	inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, ip, sizeof(ip));
	port = ntohs(clientaddr.sin_port);
	for (;;)
	{
		//接受客户端传递的信息
		ret = read(clientfd, buff, sizeof(buff)-1);
		if (ret == 0)
		{
			printf("客户端[%s: %d]断开连接\n", ip, port);
			break;
		}
		else if (ret < 0)
		{
			printf("读取失败\n");
			return -1;
		}

		buff[ret] = '\0';
		printf("recv a mag[%s: %d]: %s\n", ip, port, buff);

		//向客户端发送数据
		memset(buff, 0, sizeof(buff));
		sprintf(buff, "i got it");
		ret = write(clientfd, buff, strlen(buff));
		if (ret < 0)
		{
			printf("发送数据失败\n");
			return -1;
		}
	}
}

void child_handle(int sig)
{
	wait(NULL);
	return ;
}

int main()
{
	int pid;
	int listenfd, clientfd, ret;
	struct sockaddr_in serveraddr;
	struct sockaddr_in clientaddr;
	int clientaddrlen = sizeof(clientaddr);
	char buff[200];
	char ip[20];
	unsigned short port;

	if (signal(SIGCHLD, child_handle) == SIG_ERR)
	{
		printf("注册信号失败\n");
		return -1;
	}

	//创建套接字
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	assert(listenfd >= 0);

	//设置服务器ip地址及其端口号
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(7000);
	//serveraddr.sin_adr.s_addr = inet_addr("192.168.1.211");
	inet_pton(AF_INET, "192.168.1.211", &serveraddr.sin_addr.s_addr);

	//绑定服务器ip及其端口号
	ret = bind(listenfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	assert(ret == 0);

	//监听套接字
	ret = listen(listenfd, 20);
	assert(ret == 0);

	for (;;)
	{
		memset(&clientaddr, 0, sizeof(clientaddr));
		//接受客户端请求
		//	printf("before accept\n");
		//	clientfd = accept(listenfd, NULL, NULL);
		clientfd = accept(listenfd, (struct sockaddr*)&clientaddr, &clientaddrlen);
		if (clientfd < 0)
		{
			printf("接收客户端连接请求失败\n");
			return -1;
		}
		//	printf("after accept\n");
		inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, ip, sizeof(ip));
		port = ntohs(clientaddr.sin_port);
		printf("客户端[%s: %d]建立连接\n", ip, port);

		pid = fork();
		if (pid == 0)
		{
			close(listenfd);
			clientTask(clientfd, clientaddr);
			close(clientfd);
			exit(0);
		}
		else if (pid > 0)
		{
			close(clientfd);
			continue;
		}

		close(clientfd);
	}
	close(clientfd);
	close(listenfd);
	return 0;
}
