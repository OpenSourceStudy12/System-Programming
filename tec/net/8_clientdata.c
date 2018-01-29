#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <assert.h>

#define		LOG_REQ		10
#define 	LOG_FL		11
#define 	LOG_OK		12

struct login
{
	int type;
	char account[20];
	char passwd[20];
};

int main()
{
	int clientfd, ret;
	struct sockaddr_in serveraddr;
	char buff[200];
	struct login log;

	clientfd = socket(AF_INET, SOCK_STREAM, 0);
	assert(clientfd >= 0);

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(7000);
	inet_pton(AF_INET, "192.168.1.211", &serveraddr.sin_addr.s_addr);

	ret = connect(clientfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	assert(ret == 0);

	for(;;)
	{
		bzero(&log, sizeof(log));

		log.type = LOG_REQ;
		printf("请输入帐号： ");
		scanf("%s", log.account);
		printf("请输入密码： ");
		scanf("%s", log.passwd);
		
		ret = write(clientfd, &log, sizeof(log));
		if (ret < 0)
		{
			perror("write");
			return -1;
		}

		bzero(&log, sizeof(log));
		ret = read(clientfd, &log, sizeof(log));
		if (ret < 0)
		{
			perror("read");
			return -1;
		}
		else if (ret == 0)
		{
			printf("与服务器断开连接\n");
			return -1;
		}
		
		if (log.type == LOG_OK)
		{
			printf("登录成功\n");
		}
		else if (log.type == LOG_FL)
		{
			printf("登录失败\n");
		}
	}

	close(clientfd);
	return 0;
}
