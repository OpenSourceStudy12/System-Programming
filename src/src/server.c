#include "server.h"

int main(int argc,char** argv)
{
	int listenfd,clientfd,ret,nfound,portt;
	struct sockaddr_in serveraddr,clientaddr;
	int clientaddrlen = sizeof(clientaddr);
	char ip[20],cmd[20],port[10];
	struct epoll_event events[200],ev;
	list_t *list = NULL; //保存库户端套接字
	client_t client,*tmp;

	//数据库初始化
	if(!mysql_init(&mysql))
	{
		position(5,5,0);
		printf("数据库初始化失败..\n");
		sleep(1);
		return -1;
	}
	mysql_options(&mysql,MYSQL_SET_CHARSET_NAME,"utf8");//设置编码方式为 UTF8

	if(!mysql_real_connect(&mysql,"192.168.1.222","stu","123456","102_hotel_system_lrh",0,NULL,0))
	{	
		printf("error in :%s %d\n",__FILE__,__LINE__);
		return -1;
	}


	listenfd = socket(AF_INET,SOCK_STREAM,0);
	assert(listenfd >= 0);
	setNonblock(listenfd);
	/*
	   if(argv[1] != NULL)
	   port = atoi(argv[1]);
	   else
	   port = 7000;
	 */
	//读取ip和端口	
	if(getcnf("ip",ip) < 0)
	{
		fprintf(stdout,"配置文件读取失败\n");
		return -1;
	}
	if(getcnf("port",port) < 0)
	{
		fprintf(stdout,"配置文件读取失败\n");
		return -1;
	}

	printf("ip = %s,port = %d\n",ip,atoi(port));
	//设置ip和端口
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(atoi(port));
	inet_pton(AF_INET,ip,&serveraddr.sin_addr.s_addr);

	//绑定ip和端口到监听套接字
	ret = bind(listenfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
	assert(ret == 0);

	//监听
	ret = listen(listenfd,20);
	assert(ret == 0);

	//创建epoll文件
	epfd = epoll_create(1);
	assert(epfd >= 0);

	//添加listenfd到epfd
	ev.data.fd = listenfd;
	ev.events = EPOLLIN;
	epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev);

	//将标准输入添加到epfd
	ev.data.fd = STDIN_FILENO;
	ev.events = EPOLLIN;
	epoll_ctl(epfd,EPOLL_CTL_ADD,STDIN_FILENO,&ev);

	//线程池初始化
	pool_init(10);

	for(;;)
	{
		//等待触发
		nfound = epoll_wait(epfd,events,200,-1);	
		if(nfound < 0)
		{
			perror("epoll_wait");
			continue;
		}
		if(nfound == 0)
		{
			printf("超时\n");
			continue;
		}
		//遍历触发
		for(int i=0;i<nfound;i++)
		{
			//标准输入触发
			if(events[i].data.fd == STDIN_FILENO)
			{
				ret = read(STDIN_FILENO,cmd,sizeof(cmd));	
				if(ret < 0)
				{
					perror("read");
					continue;
				}
				cmd[ret-1] = '\0';
				if(!strcmp(cmd,"exit") || !strcmp(cmd,"quit"))//服务器退出
				{
					close(listenfd);
					mysql_close(&mysql);
					pool_destroy();
					client_close(list);//关闭客户端套接字
					exit(0);
				}
				else
				{
					printf("---------------------------------------------------------------------------------\n");
					system(cmd);//执行系统命令
					printf("---------------------------------------------------------------------------------\n");
					continue;
				}
			}
			//客户端连接请求触发
			else if(events[i].data.fd == listenfd)
			{
				for(;;)
				{
					//等待客户端连接请求，非阻塞，立即返回  读取错误或数据未到达----》clientfd < 0
					clientfd = accept(listenfd,(struct sockaddr*)&clientaddr,&clientaddrlen);
					if(clientfd < 0)
					{
						//非阻塞错误
						if(errno == EAGAIN || errno == EWOULDBLOCK)
							break;
						perror("accept");
						continue;
					}

					getpeername(clientfd,(struct sockaddr*)&clientaddr,&clientaddrlen);
					client.port = ntohs(clientaddr.sin_port);
					inet_ntop(AF_INET,&clientaddr.sin_addr.s_addr,client.ip,sizeof(client.ip));
					client.clientfd = clientfd; 
				//	hour_min_sec(&client.time.hour,&client.time.min,&client.time.sec);//记录客户端发消息时间
					list = list_create(list,&client,sizeof(client));//保存客户端信息

					fprintf(stdout,"客户端[%s:%d]建立连接\n",client.ip,client.port);

					ev.data.fd = clientfd;
					ev.events = EPOLLIN | EPOLLET;//设置套结字文件为边缘触发
					epoll_ctl(epfd,EPOLL_CTL_ADD,clientfd,&ev);//将通信套接字文件添加到epfd
				//	setNonblock(clientfd);
				//	continue;
				}
			}
			//客户端通信出发
			else if(events[i].events & EPOLLIN)
			{
				clientfd = events[i].data.fd;
			/*	tmp = list_compare(list,&clientfd,clientfd_compare);
				if(tmp != NULL)
				{
					hour_min_sec(&tmp->time.hour,&tmp->timee.min,&tmp->time.sec);
				}*/
				communication(clientfd);	
			}
		}
	}
	
	close(listenfd);//关闭监听套接字
	mysql_close(&mysql);//关闭数据库
	pool_destroy();//销毁线程池
	client_close(list);//关闭客户端套接字
}
