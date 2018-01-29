#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <assert.h>
#include "package.h"

void cli_login(int clientfd)
{
	package_t head = {"v1.0", "p1.0", CLI_LOG_REQ, sizeof(login_t)};
	login_t log;
	int ret;

	printf("请输入帐号：\n");
	scanf("%s", log.account);
	printf("请输入密码:\n");
	scanf("%s", log.passwd);

	void *pack = packageHead(head, &log);
	ret = write(clientfd, pack, sizeof(head)+head.length);
	if (ret < 0)
	{
		perror("write");
		return;
	}

	ret = read(clientfd, &head, sizeof(head));
	if (ret < 0)
	{
		perror("read");
		return ;
	}
	else if (ret == 0)
	{
		printf("断开连接\n");
		close(clientfd);
		exit(0);
	}

	if (head.type == CLI_LOG_OK)
		printf("登录成功\n");
	else if (head.type == CLI_LOG_FL)
	{
		printf("登录失败\n");
	}
	free(pack);
}

int main()
{
	int clientfd, ret, opt;
	struct sockaddr_in serveraddr;
	char buff[200];

	clientfd = socket(AF_INET, SOCK_STREAM, 0);
	assert(clientfd >= 0);

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(7000);
	inet_pton(AF_INET, "192.168.1.211", &serveraddr.sin_addr.s_addr);

	ret = connect(clientfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	assert(ret == 0);

	for(;;)
	{
		while (1)
		{
			printf("请选择:\n");
			printf("1.登录\n");
			printf("2.注册\n");
			printf("0.退出\n");
			ret = scanf("%d", &opt);
			if (ret == 0)
			{
				memset(&opt, 0, sizeof(opt));
			}
			else
				break;
		}
		if (opt == 1)
		{
			cli_login(clientfd);
		}
		else if (opt == 2)
		{
		}
		else if (opt == 0)
		{
			return 0;
		}
	}

	close(clientfd);
	return 0;
}
