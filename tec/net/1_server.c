#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <assert.h>

int main()
{
	int listenfd, clientfd, ret;
	struct sockaddr_in serveraddr;
	char buff[200];

	//创建套接字
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	assert(listenfd >= 0);

	//设置自身ip和端口-->用于绑定
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(7000);	//将小端字节装换成大端
	inet_pton(AF_INET, "192.168.1.211", &serveraddr.sin_addr.s_addr);

	//绑定自身ip和端口
	ret = bind(listenfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	assert(ret == 0);

	//监听
	ret = listen(listenfd, 20);
	assert(ret == 0);

	//接受客户端的连接请求
	clientfd = accept(listenfd, NULL, NULL);
	if (clientfd < 0)
	{
		perror("accept");
		return -1;
	}

	//连接建立，进行通信
	memset(buff, 0, sizeof(buff));
	ret = read(clientfd, buff, sizeof(buff)-1);
	if (ret < 0)
	{
		printf("read fail\n");
		return -1;
	}
	else if (ret == 0)
	{
		printf("与客户端断开连接\n");
		return -1;
	}
	buff[ret] = 0;
	printf("recv a msg: %s\n", buff);

	memset(buff, 0, sizeof(buff));
	sprintf(buff, "this is a server");
	ret = write(clientfd, buff, strlen(buff));
	if (ret < 0)
	{
		printf("write fail\n");
		return -1;
	}


	close(clientfd);
	close(listenfd);
	return 0;
}
