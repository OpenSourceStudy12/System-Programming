#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int main()
{
	struct hostent *host;
	char ip[20];

	host = gethostbyname("www.baidu.com");
	if (host == NULL)
	{
		printf("获取ip地址失败\n");
		return -1;
	}

	while (*host->h_addr_list)
	{
		inet_ntop(AF_INET, *host->h_addr_list, ip, sizeof(ip));
		printf("%s\n", ip);
		host->h_addr_list++;
	}

	return 0;
}
