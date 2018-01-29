#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <assert.h>
#include "pool.h"

struct Node
{
	int clientfd;
	int port;
	char ip[20];
	struct Node *next;
};

struct Node *head = NULL;
pthread_mutex_t mutex;

void *client_thread(void *argv)
{
	int clientfd, ret, port;
	char ip[20], buff[200];
	struct sockaddr_in clientaddr;
	int clientaddrlen = sizeof(clientaddr);
	struct Node *temp, *current;
	char buff_[300];

	clientfd = *(int*)argv;
	getpeername(clientfd, (struct sockaddr*)&clientaddr, &clientaddrlen);

	inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, ip, sizeof(ip));
	port = ntohs(clientaddr.sin_port);

	for (;;)
	{
		bzero(buff, sizeof(buff));
		ret = read(clientfd, buff, sizeof(buff)-1);
		if (ret < 0)
		{
			perror("read");
			close(clientfd);
			break;
		}
		else if (ret == 0)
		{
			printf("客户端[%s:%d]断开连接\n", ip, port);
			break;
		}
		buff[ret] = 0;
		printf("客户端[%s:%d]发送消息::%s\n", ip, port, buff);

		pthread_mutex_lock(&mutex);
		for (temp = head; temp != NULL; temp = temp->next)
		{
			if (temp->clientfd == clientfd)
				continue;
			sprintf(buff_, "[%s:%d]::%s", temp->ip, temp->port, buff);
			ret = write(temp->clientfd, buff_, strlen(buff_));
			if (ret < 0)
			{
				perror("write");
				continue;
			}
		}
		pthread_mutex_unlock(&mutex);
	}

	pthread_mutex_lock(&mutex);
	temp = head;
	while (temp != NULL)
	{
		if (head->clientfd == clientfd)
		{
			head = head->next;
			free(temp);
			break;
		}
		else if (temp->next->clientfd == clientfd)
		{
			current = temp->next;
			temp->next = current->next;
			free(current);
			break;
		}
		temp = temp->next;
	}
	pthread_mutex_unlock(&mutex);
	
	close(clientfd);
	return NULL;
}

int main()
{
	int listenfd, clientfd, ret, port;
	struct sockaddr_in serveraddr, clientaddr;
	char ip[20], buff[200];
	int clientaddrlen = sizeof(clientaddr);
	struct Node *current;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	assert(listenfd >= 0);

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(7000);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);//0.0.0.0

	ret = bind(listenfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	assert(ret == 0);

	ret = listen(listenfd, 20);
	assert(ret == 0);

	pthread_mutex_init(&mutex, NULL);
	ret = pool_init(30);
	assert(ret == 0);

	for (;;)
	{
		clientfd = accept(listenfd, (struct sockaddr*)&clientaddr, &clientaddrlen);
		if (clientfd < 0)
		{
			perror("accept");
			continue;
		}

		port = ntohs(clientaddr.sin_port);
		inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, ip, sizeof(ip));
		printf("客户端[%s:%d]建立连接\n", ip, port);

		current = malloc(sizeof(struct Node));
		assert(current != NULL);

		current->clientfd = clientfd;
		current->port = port;
		strcpy(current->ip, ip);
		current->next = NULL;

		if (head == NULL)
			head = current;
		else 
		{
			current->next = head;
			head = current;
		}

		ret = pool_add_worker(client_thread, &clientfd, sizeof(clientfd));
		if (ret < 0)
		{
			fprintf(stderr, "添加任务失败\n");
			continue;
		}
	}

	pthread_mutex_destroy(&mutex);
	pool_destroy();
	close(listenfd);
	return 0;
}
