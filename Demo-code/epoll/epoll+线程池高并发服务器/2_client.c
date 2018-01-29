#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>

int main()
{
	int clientfd, ret;
	struct sockaddr_in serveraddr;
	char buff[200];

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
		printf("please input a msg:\n");
		ret = read(STDIN_FILENO, buff, sizeof(buff));
		buff[ret-1] = 0;

		ret = write(clientfd, buff, strlen(buff));
		if (ret < 0)
		{
			printf("发送数据失败\n");
			return -1;
		}

		//接收数据
		memset(buff, 0, sizeof(buff));
		ret = read(clientfd, buff, sizeof(buff)-1);
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
		buff[ret] = 0;
		printf("recv a msg: %s\n", buff);
	}

	close(clientfd);
	return 0;
}
