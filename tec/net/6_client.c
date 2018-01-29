#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main()
{
	int clientfd, ret;
	struct sockaddr_in serveraddr, clientaddr;
	char buff[200];

	clientfd = socket(AF_INET, SOCK_STREAM, 0);
	assert(clientfd >= 0);

	clientaddr.sin_family = AF_INET;
	clientaddr.sin_port = htons(8080);
	inet_pton(AF_INET, "192.168.1.211", &clientaddr.sin_addr.s_addr);

	ret = bind(clientfd, (struct sockaddr*)&clientaddr, sizeof(clientaddr));
	assert(ret == 0);

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(7000);
	inet_pton(AF_INET, "192.168.1.211", &serveraddr.sin_addr.s_addr);

	ret = connect(clientfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	assert(ret == 0);

	for (;;)
	{
		bzero(buff, sizeof(buff));
		printf("please input a msg:\n");
		ret = read(STDIN_FILENO, buff, sizeof(buff));
		if (ret < 0)
		{
			perror("read");
			continue;
		}

		ret = write(clientfd, buff, ret-1);
		if (ret < 0)
		{
			perror("write");
			continue;
		}

		bzero(buff, sizeof(buff));
		ret = read(clientfd, buff, sizeof(buff)-1);
		if (ret < 0)
		{
			perror("read");
			continue;
		}
		else if (ret == 0)
		{
			printf("与服务器断开连接\n");
			break;
		}
		buff[ret] = '\0';
		printf("recv a msg: %s\n", buff);
	}

	close(clientfd);
	return 0;
}
