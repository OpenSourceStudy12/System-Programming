#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

int main()
{
	struct hostent *host;
	char ip[20];

	host = gethostbyname("www.baidu.com");
	if (host == NULL)
	{
		printf("获取失败\n");
		return -1;
	}

	int type = host->h_addrtype;
	char **p = host->h_addr_list;

	for (; *p != NULL; p++)
	{
		inet_ntop(type, *p, ip, sizeof(ip));
		printf("ip = %s\n", ip);
	}

	return 0;
}
