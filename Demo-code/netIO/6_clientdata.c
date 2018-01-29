#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include "package.h"

int regist_client(int clientfd)
{
	int ret;
	package_t pack = {"v1.0", "p1.0", CLI_REG_REQ, sizeof(regist_t)};
	regist_t reg;

	printf("请输入帐号:\n");
	scanf("%s", reg.account);
	printf("请输入密码:\n");
	scanf("%s", reg.passwd);
	printf("请输入姓名:\n");
	scanf("%s", reg.name);
	printf("请输入电话:\n");
	scanf("%s", reg.tel);
	printf("请输入年龄:\n");
	scanf("%d", &reg.age);

	ret = packageHeadSend(clientfd, pack, &reg);
	if (ret < 0)
	{
		printf("发送消息失败\n");
		return -1;
	}

	ret = read(clientfd, &pack, sizeof(pack));
	if (ret < 0)
	{
		perror("read");
		return -1;
	}
	else if (ret == 0)
	{
		printf("与服务器断开连接\n");
		exit(0);
	}

	if (pack.type == CLI_REG_OK)
	{
		printf("注册成功\n");
	}
	else if (pack.type == CLI_REG_FL)
		printf("注册失败\n");
}

int login_client(int clientfd)
{
	package_t pack = {"v1.0", "p1.0", CLI_LOG_REQ, sizeof(login_t)};
	login_t log;
	int ret;

	printf("请输入帐号:\n");
	scanf("%s", log.account);
	printf("请输入密码:\n");
	scanf("%s", log.passwd);

/*	void *p = packageHead(pack, &log);
	int ret = write(clientfd, p, sizeof(pack)+pack.length);
	if (ret < 0)
	{
		perror("write");
		return -1;
	}*/
	ret = packageHeadSend(clientfd, pack, &log);
	if (ret < 0)
	{
		printf("发送消息失败\n");
		return -1;
	}

	ret = read(clientfd, &pack, sizeof(pack));
	if (ret < 0)
	{
		perror("read");
		return -1;
	}
	else if (ret == 0)
	{
		printf("与服务器断开连接\n");
		exit(0);
	}

	if (pack.type == CLI_LOG_OK)
	{
		printf("登录成功\n");
	}
	else if (pack.type == CLI_LOG_FL)
	{
		printf("登录失败\n");
	}
}

int main()
{
	int clientfd, ret;
	struct sockaddr_in serveraddr;
	char buff[200];
	int opts;

	//创建套接字
	clientfd = socket(AF_INET, SOCK_STREAM, 0);
	assert(clientfd >= 0);

	//设置服务器ip以及端口信息
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(7000);
	inet_pton(AF_INET, "127.0.0.1", &serveraddr.sin_addr.s_addr);

	//发送连接请求
	ret = connect(clientfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	perror("connect");
	assert(ret == 0);

	for (;;)
	{
		printf("请选择：\n");
		printf("1.登录\n2.注册\n0.退出\n");
		scanf("%d", &opts);
		
		switch (opts)
		{
			case 1:
				login_client(clientfd);
				break;
			case 2:
				regist_client(clientfd);
				break;
			case 0:
				close(clientfd);
				exit(0);
			default:
				printf("输入有误\n");
				break;
		}
	}

	close(clientfd);
	return 0;
}
