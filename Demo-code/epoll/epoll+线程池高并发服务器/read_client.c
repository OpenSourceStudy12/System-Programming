#include"read_client.h"

void *read_thread(void *argv)
{
	int clientfd = *(int*)argv;
	int ret, port,epollfd;
	struct sockaddr_in serveraddr, clientaddr;
	int clientaddrlen = sizeof(clientaddr);
	char buff[200], ip[20], cmd[50];
	struct epoll_event ev, events[200];

	memset(buff, 0, sizeof(buff));
	getpeername(clientfd, (struct sockaddr*)&clientaddr, &clientaddrlen);
	port = ntohs(clientaddr.sin_port);
	inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, ip, sizeof(ip));
//	pthread_detach(pthread_self());
//	printf("thread[%lu]\n", pthread_self());

	ret = read(clientfd, buff, sizeof(buff)-1);
	if (ret == 0)
	{
		printf("客户端[%s:%d]断开连接\n", ip, port);
		ev.data.fd = clientfd;
		ev.events = EPOLLIN | EPOLLET;
		epoll_ctl(epollfd, EPOLL_CTL_DEL, clientfd, &ev);
		close(clientfd);
	//	continue;
		return NULL;
	}
	else if (ret < 0)
	{
		perror("read");
	//	continue;
		return NULL;
	}

	buff[ret] = 0;
	printf("客户端[%s:%d]发送数据:%s\n", ip, port, buff);

	ev.data.fd = clientfd;
	ev.events = EPOLLOUT | EPOLLET;
	epoll_ctl(epollfd, EPOLL_CTL_MOD, clientfd, &ev);//修改标识符可写，用于下次循环发送数据

}
