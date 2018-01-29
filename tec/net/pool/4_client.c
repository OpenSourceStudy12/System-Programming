#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>

void *write_handle(void *argv)
{
	int clientfd = (int)argv;
	char buff[300];
	int ret;

	for (;;)
	{
		bzero(buff, sizeof(buff));
		ret = read(clientfd, buff, sizeof(buff)-1);
		if (ret < 0)
		{
			perror("read");
			return NULL;
		}
		else if (ret == 0)
		{
			printf("与服务器断开连接\n");
			return NULL;
		}
		buff[ret] = 0;
		printf("recv a msg%s\n", buff);
	}
}

int main()
{
	int clientfd, ret;
	struct sockaddr_in serveraddr;
	char buff[200];
	pthread_t pth;

	clientfd = socket(AF_INET, SOCK_STREAM, 0);
	assert(clientfd >= 0);

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(7000);
	inet_pton(AF_INET, "192.168.1.91", &serveraddr.sin_addr.s_addr);

	ret = connect(clientfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	assert(ret == 0);

	ret = pthread_create(&pth, NULL, write_handle, (void*)clientfd);
	assert(ret == 0);

	for(;;)
	{
		bzero(buff, sizeof(buff));
		printf("请输入：\n");
		ret = read(STDIN_FILENO, buff, sizeof(buff));
		if (ret < 0)
		{
			perror("read");
			return -1;
		}

		ret = write(clientfd, buff, ret-1);
		if (ret < 0)
		{
			perror("write");
			return -1;
		}

	}

	close(clientfd);
	return 0;
}
