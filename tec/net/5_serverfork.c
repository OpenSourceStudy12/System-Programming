#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <assert.h>
#include <sys/wait.h>

#define		LOG_REQ		10
#define 	LOG_FL		11
#define 	LOG_OK 		12

struct login
{
	int type;
	char account[20];
	char passwd[20];
};

int toTask(int clientfd, struct sockaddr_in clientaddr)
{
	int port, ret;
	char ip[20];
	char buff[200];
	struct login log;

	port = ntohs(clientaddr.sin_port);
	inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, ip, sizeof(ip));
	while (1)
	{
		bzero(&log, sizeof(log));
		ret = read(clientfd, &log, sizeof(log));
		if (ret < 0)
		{
			perror("read");
			return -1;
		}
		else if (ret == 0)
		{
			printf("与客户端[%s:%d]断开连接\n", ip, port);
			break;
		}
		if (strcmp(log.account, "admin") == 0 && strcmp(log.passwd, "123456") == 0)
			log.type = LOG_OK;
		else
			log.type = LOG_FL;

		printf("type = %d\n", log.type);
		ret = write(clientfd, &log, sizeof(log));
		if (ret < 0)
		{
			perror("write");
			return -1;
		}
	}
}

void sig_handle(int num)
{
	wait(NULL);
	return;
}

int main()
{
	int listenfd, clientfd, ret, port;
	struct sockaddr_in serveraddr, clientaddr;
	int clientaddrlen = sizeof(clientaddr);
	char buff[200];
	char ip[20];
	pid_t pid;

	if (signal(SIGCHLD, sig_handle) == SIG_ERR)
	{
		printf("更改信号默认处理失败\n");
		return -1;
	}

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
		clientfd = accept(listenfd, (struct sockaddr*)&clientaddr, &clientaddrlen);//&sizeof(clientaddr)
		if (clientfd < 0)
		{
			perror("accept");
			continue;
		}

		port = ntohs(clientaddr.sin_port);
		inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, ip, sizeof(ip));
		printf("客户端[%s:%d]建立连接\n", ip, port);

		pid = fork();
		if (pid == 0)
		{
			close(listenfd);
			toTask(clientfd, clientaddr);
			return 0;
		}
		else if (pid > 0)
		{
			close(clientfd);
		}
		else 
		{
			perror("fork");
			return -1;
		}
	}

	close(clientfd);
	close(listenfd);
	return 0;
}
