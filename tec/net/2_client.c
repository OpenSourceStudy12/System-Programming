#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <assert.h>

int main()
{
	int clientfd, ret;
	struct sockaddr_in serveraddr;
	char buff[200];

	//创建套接字
	clientfd = socket(AF_INET, SOCK_STREAM, 0);
	assert(clientfd >= 0);

	//需要建立连接的服务器的ip和端口
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(7000);
	inet_pton(AF_INET, "192.168.1.211", &serveraddr.sin_addr.s_addr);

	//发送连接请求
	ret = connect(clientfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	assert(ret == 0);

	bzero(buff, sizeof(buff));
	sprintf(buff, "hello world");
	ret = write(clientfd, buff, strlen(buff));
	if (ret < 0)
	{
		printf("write fail\n");
		return -1;
	}

	bzero(buff, sizeof(buff));
	ret = read(clientfd, buff, sizeof(buff)-1);
	if (ret < 0)
	{
		printf("read fail\n");
		return -1;
	}
	else if (ret == 0)
	{
		printf("与服务器断开连接\n");
		return -1;
	}
	buff[ret] = 0;
	printf("recv a msg: %s\n", buff);

	close(clientfd);
	return 0;
}
