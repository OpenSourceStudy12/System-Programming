#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>

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

int main()
{
	int clientfd, ret;
	struct sockaddr_in serveraddr;
	char buff[200];
	INFO info;

	//创建套接字
	clientfd = socket(AF_INET, SOCK_STREAM, 0);
	assert(clientfd >= 0);

	//设置服务器ip以及端口信息
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(7000);
	inet_pton(AF_INET, "192.168.1.128", &serveraddr.sin_addr.s_addr);

	//发送连接请求
	ret = connect(clientfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	assert(ret == 0);

	for (;;)
	{
		//发送数据
		memset(buff, 0, sizeof(buff));
		printf("please input your account:\n");
		scanf("%s", info.account);
		printf("please input your passwd: \n");
		scanf("%s", info.passwd);
		info.type = CLI_LOG;

		ret = write(clientfd, &info, sizeof(info));
		if (ret < 0)
		{
			printf("发送数据失败\n");
			return -1;
		}

		//接收数据
		memset(&info, 0, sizeof(info));
		ret = read(clientfd, &info, sizeof(info));
		if (ret == 0)
		{
			printf("服务器断开连接\n");
			break;
		}
		else if (ret < 0)
		{
			printf("读取数据失败\n");
			return -1;
		}

		if (info.type == CLI_LOG_OK)
		{
			printf("登录成功\n");
		}
		else if (info.type == CLI_LOG_FL)
		{
			printf("登录失败\n");
		}
	}

	close(clientfd);
	return 0;
}
