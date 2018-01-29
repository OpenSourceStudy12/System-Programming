#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <assert.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main()
{
	int sockfd, ret, port;
	struct sockaddr_in serveraddr;
	char buff[200];

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	assert(sockfd >= 0);

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(8080);
	inet_pton(AF_INET, "192.168.1.211", &serveraddr.sin_addr.s_addr);

	for (;;)
	{
		bzero(buff, sizeof(buff));
		printf("请输入:\n");
		ret = read(STDIN_FILENO, buff, sizeof(buff));
		if (ret < 0)
		{
			perror("read");
			continue;
		}
		buff[ret-1] = 0;

		ret = sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
		if (ret < 0)
		{
			perror("sendto");
			continue;
		}

		bzero(buff, sizeof(buff));
		ret = recvfrom(sockfd, buff, sizeof(buff)-1, 0, NULL, NULL);
		if (ret < 0)
		{
			perror("recvfrom");
			continue;
		}

		printf("%s\n", buff);
	}

	close(sockfd);
	return 0;
}
