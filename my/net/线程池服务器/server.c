#include"pool.h"

void* handle(void* argv)
{
	char buff[200];
	char ip[20];
	int clientfd,port,ret;
	struct sockaddr_in cli_add;
	int cli_addlen = sizeof(cli_add);
#if 1
	memcpy(&clientfd,argv,sizeof(clientfd));
	memcpy(&cli_add,argv+sizeof(clientfd),sizeof(cli_add));
#endif

#if 0
	clientfd = *(int*)argv;
	getpeername(clientfd,(struct sockaddr*)&cli_add,&cli_addlen);
#endif
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


int main()
{
	int listenfd,clientfd,ret;
	struct sockaddr_in serv_add,cli_add;
	int cli_addlen;
	char buff[300];

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
	
	//创建线程池
	ret = pool_init(10);
	if(ret < 0)
	{
		fprintf(stderr,"pool init fail\n");
		return -1;
	}

	sleep(2);//延时等子线程创建完
	
	for(;;)
	{
		//接收客户端连接请求
		cli_addlen = sizeof(struct sockaddr_in);
		clientfd = accept(listenfd,(struct sockaddr*)&cli_add,&cli_addlen);//等待新的连接(有阻塞)
		assert(clientfd >= 0);
#if 1	
		memset(buff,0,sizeof(buff));

		memcpy(buff,&clientfd,sizeof(clientfd));
		memcpy(buff+sizeof(clientfd),&cli_add,sizeof(cli_add));
		printf("buff = %s\n",buff);

		func_add(handle,buff,sizeof(buff));
#endif

#if 0
		func_add(handle,&clientfd,sizeof(clientfd));
#endif
		if(ret < 0)
		{
			perror("thread create");
			return -1;
		}
	}

	pool_destroy();
	close(listenfd);
	return 0;

}
