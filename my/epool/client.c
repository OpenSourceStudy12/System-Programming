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

	clientfd = socket(AF_INET, SOCK_STREAM, 0);
	assert(clientfd >= 0);

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(7000);
	inet_pton(AF_INET, "192.168.1.91", &serveraddr.sin_addr.s_addr);

	ret = connect(clientfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	assert(ret == 0);

	for(;;)
	{
		bzero(buff, sizeof(buff));
		printf("请输入：\n");
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
			close(clientfd);
			return -1;
		}
		buff[ret] = 0;
		printf("recv a msg: %s\n", buff);
	}

	close(clientfd);
	return 0;
}
