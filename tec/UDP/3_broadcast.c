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
	struct sockaddr_in serveraddr;
	char buff[200];
	int opt = 1;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	assert(sockfd >= 0);

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(8000);
	inet_pton(AF_INET, "192.168.1.255", &serveraddr.sin_addr.s_addr);

	ret = setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt));
	if (ret < 0)
	{
		perror("setsockopt");
		return -1;
	}

	for (int i = 0;; ++i)
	{
		sprintf(buff, "this is the %d times", i);
		ret = sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
		if (ret < 0)
		{
			perror("sendto");
			return -1;
		}
		sleep(1);
	}

	return 0;
}
