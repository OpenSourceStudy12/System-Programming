#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("too few argument %s: \n", argv[0]);
		return -1;
	}

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
	serveraddr.sin_port = htons(atoi(argv[1]));
	inet_pton(AF_INET, "192.168.1.211", &serveraddr.sin_addr.s_addr);

	ret = connect(clientfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	assert(ret == 0);

	memset(buff, 0, sizeof(buff));
	printf("please input a msg: \n");
	ret = read(STDIN_FILENO, buff, sizeof(buff)-1);
	buff[ret-1] = '\0';

	ret = write(clientfd, buff, strlen(buff));
	assert(ret > 0);

	memset(buff, 0, sizeof(buff));
	ret = read(clientfd, buff, sizeof(buff)-1);
	if (ret == 0)
	{
		printf("与服务器断开连接\n");
		return -1;
	}
	else if (ret < 0)
	{
		perror("read");
		return -1;
	}
	buff[ret] = '\0';
	printf("recv a msg:%s\n", buff);

	close(clientfd);
	return 0;
}
