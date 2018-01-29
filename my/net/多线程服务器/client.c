#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>
#include<string.h>
#include<assert.h>

int main()
{
	int clientfd,ret;
	struct sockaddr_in serv_add;
	char buff[200];

	//创建套接字
	clientfd = socket(AF_INET,SOCK_STREAM,0);
	assert(clientfd >= 0);

	//指定服务器地址和端口
	serv_add.sin_family = AF_INET;
	serv_add.sin_port = htons(7000);
	inet_pton(AF_INET,"192.168.1.211",&serv_add.sin_addr.s_addr);

	//连接服务器
	ret = connect(clientfd,(struct sockaddr*)&serv_add,sizeof(serv_add));
	assert(ret == 0);

	while(1)
	{
		//连接建立，进行通信
		memset(buff,0,sizeof(buff));
		printf("input a message:\n");
		ret = read(STDOUT_FILENO,buff,sizeof(buff));
		//sprintf(buff,"%s","hello world");
		if(ret < 0)
		{
			printf("read fail\n");
			continue;
		}
		ret = write(clientfd,buff,ret-1);
		if(ret < 0)
		{
			fprintf(stderr,"write fail");
			continue;
		}

		//	bzero(buff,sizeof(buff));
/*		ret = read(clientfd,buff,sizeof(buff)-1);
		if(ret < 0)
		{
			printf("read fail\n");
			continue;
		}
		else if(ret == 0)
		{
			printf("与服务器连接断开\n");
			break;
		}
		buff[ret] = 0;
		printf("receive a mesage : %s\n",buff);
*/
	}
	close(clientfd);
	return 0;
}
