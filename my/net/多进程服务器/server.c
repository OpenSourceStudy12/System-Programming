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
#include<sys/wait.h>

void task_deal(int clientfd,struct sockaddr_in cli_add)
{
	char buff[200];
	char ip[20];
	int port,ret;
	port = ntohs(cli_add.sin_port);
	inet_ntop(AF_INET,&cli_add.sin_addr.s_addr,ip,sizeof(ip));
	printf("客户端[%s:%d]建立连接\n",ip,port);

	while(1)
	{
		//连接建立，进行通信
		memset(buff,0,sizeof(buff));
		ret = read(clientfd,buff,sizeof(buff)-1);
		if(ret < 0)
		{
			fprintf(stderr,"read fail");
			continue;
		}
		else if(ret == 0)
		{
			fprintf(stdout,"与客户端[%s:%d]连接断开\n",ip,port);
			break;
		}
		buff[ret-1] = 0;
		printf("receive a message : %s\n",buff);

		//	bzero(buff,sizeof(buff));
		sprintf(buff,"%s","this is server");
		ret = write(clientfd,buff,strlen(buff));
		if(ret < 0)
		{
			printf("write fail\n");
			continue;
		}
	}
	close(clientfd);
}

void sig_handle()
{
	wait(NULL);
	return ;
}

int main()
{
	int listenfd,clientfd,ret;
	struct sockaddr_in serv_add,cli_add;
	int cli_addlen = sizeof(cli_add);
	pid_t pid;

	if(signal(SIGCHLD,sig_handle) == SIG_ERR)
	{
		fprintf(stderr,"信号注册失败\n");	
		return -1;
	}

	//创建套接字
	listenfd = socket(AF_INET,SOCK_STREAM,0);
	assert(listenfd >= 0);

	//指定服务器地址和端口
	serv_add.sin_family = AF_INET;
	serv_add.sin_port = htons(7000);
	inet_pton(AF_INET,"192.168.1.91",&serv_add.sin_addr.s_addr);

	//绑定ip和端口到套接字
	ret = bind(listenfd,(struct sockaddr*)&serv_add,sizeof(serv_add));
	assert(ret == 0);

	//监听
	ret = listen(listenfd,20);
	assert(ret >= 0);
	while(1)
	{
		//接收客户端连接请求
		clientfd = accept(listenfd,(struct sockaddr*)&cli_add,&cli_addlen);//等待新的连接
		assert(clientfd >= 0);

		pid = fork();
		if(pid == 0)
		{
			close(listenfd);
			task_deal(clientfd,cli_add);
			return 0;
		}
		else if(pid > 0)
		{
			close(clientfd);
		}
		else
		{
			perror("fork");
			return -1;
		}
	}
	close(listenfd);
	return 0;
}
