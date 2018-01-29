#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main()
{
	int sockfd, ret;
	char buff[200];
	struct sockaddr_in broadaddr;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	assert(sockfd >= 0);

	broadaddr.sin_family = AF_INET;
	broadaddr.sin_port = ntohs(8000);
	broadaddr.sin_addr.s_addr = INADDR_ANY;

	ret = bind(sockfd, (struct sockaddr*)&broadaddr, sizeof(broadaddr));
	assert(ret == 0);

	for (;;)
	{
		bzero(buff, sizeof(buff));
		ret = recvfrom(sockfd, buff, sizeof(buff)-1, 0, NULL, NULL);
		if (ret < 0)
		{
			perror("recvfrom");
			return -1;
		}
		printf("recv a msg: %s\n", buff);
	}

	close(sockfd);
	return 0;
}
