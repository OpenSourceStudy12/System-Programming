#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>

void *client_thread(void *argv)
{
	int clientfd, ret, port;
	char ip[20], buff[200];
	struct sockaddr_in clientaddr;

	memcpy(&clientfd, argv, sizeof(clientfd));
	memcpy(&clientaddr, argv+sizeof(clientfd), sizeof(clientaddr));

	port = ntohs(clientaddr.sin_port);
	inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, ip, sizeof(ip));

	pthread_detach(pthread_self());
	for (;;)
	{
		bzero(buff, sizeof(buff));
	//	ret = read(clientfd, buff, 3);	字节流协议
		ret = read(clientfd, buff, sizeof(buff)-1);
		if (ret < 0)
		{
			perror("read");
			continue;
		}
		else if (ret == 0)
		{
			printf("客户端[%s:%d]断开连接\n", ip, port);
			break;
		}
		buff[ret] = 0;
		printf("recv a msg[%s:%d]:: %s\n", ip, port, buff);

		bzero(buff, sizeof(buff));
		sprintf(buff, "this is a thread server");
		ret = write(clientfd, buff, strlen(buff));
		if (ret < 0)
		{
			perror("write");
			continue;
		}
	}

	close(clientfd);
	return NULL;
}

int main()
{
	int listenfd, clientfd, ret, port;
	struct sockaddr_in serveraddr, clientaddr;
	char buff[200], ip[20];
	int clientaddrlen = sizeof(clientaddr);
	pthread_t pth;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	assert(listenfd >= 0);

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(7000);
	inet_pton(AF_INET, "192.168.1.211", &serveraddr.sin_addr.s_addr);

	ret = bind(listenfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	assert(ret == 0);

	ret = listen(listenfd, 20);
	assert(ret == 0);

	for (;;)
	{
		clientfd = accept(listenfd, (struct sockaddr*)&clientaddr, &clientaddrlen);
		if (clientfd < 0)
		{
			fprintf(stderr, "accept fail\n");
			continue;
		}

		port = ntohs(clientaddr.sin_port);
		inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, ip, sizeof(ip));
		printf("客户端[%s:%d]建立连接\n", ip, port);

		memset(buff, 0, sizeof(buff));
		memcpy(buff, &clientfd, sizeof(clientfd));
		memcpy(buff+sizeof(clientfd), &clientaddr, sizeof(clientaddr));

		ret = pthread_create(&pth, NULL, client_thread, buff);
		if (ret < 0)
		{
			perror("pthread_create");
			continue;
		}
	}

	close(listenfd);
	return 0;
}
