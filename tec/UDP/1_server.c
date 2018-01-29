#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main()
{
	int sockfd, ret, port;
	struct sockaddr_in serveraddr, clientaddr;
	int clientaddrlen = sizeof(clientaddr);
	char buff[200], ip[20];

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	assert(sockfd >= 0);

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(8080);
	inet_pton(AF_INET, "192.168.1.211", &serveraddr.sin_addr.s_addr);

	ret = bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	assert(ret == 0);

	for (;;)
	{
		bzero(buff, sizeof(buff));
		ret = recvfrom(sockfd, buff, sizeof(buff)-1, 0, (struct sockaddr*)&clientaddr, &clientaddrlen);
		if (ret < 0)
		{
			perror("recvfrom");
			continue;
		}
		port = ntohs(clientaddr.sin_port);
		inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, ip, sizeof(ip));
		printf("客户端[%s:%d]发送消息::%s\n", ip, port, buff);

		bzero(buff, sizeof(buff));
		sprintf(buff, "this is UDP server");
		ret = sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr*)&clientaddr, clientaddrlen);
		if (ret < 0)
		{
			perror("sendto");
			continue;
		}
	}

	close(sockfd);
	return 0;
}
