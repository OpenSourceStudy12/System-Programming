#include "package.h"

void* package(pack_head_t head,void* data)
{
	void* pack_head = malloc(sizeof(pack_head_t)+head.length);
	assert(pack_head != NULL);

	memcpy(pack_head,&head,sizeof(pack_head_t));
	if(data != NULL)
		memcpy(pack_head+sizeof(pack_head_t),data,head.length);
	return pack_head;
}


int package_send(int sockfd,short type,int opt,int length,void* data)
{
	int ret;
	pack_head_t head;

	head.type = type;
	head.opt = opt;
	head.length = length;

	void* pack = package(head,data);
	
	ret = write(sockfd,pack,sizeof(pack_head_t)+head.length);
	if(ret < 0)
		ret = -1;
	ret = 0;
	free(pack);
	return 0;
}

int package_list_send(int socketfd,short type,int opt,int length,int num,list_t* list_head)
{
	int ret;
	pack_head_t head;
	void *tmp,*temp;
	void* pack;

	head.type = type;
	head.opt = opt;
	head.length = length*num + sizeof(num);
	
	temp = malloc(length*num + sizeof(num));
	assert(temp != NULL);

//	printf("-----------------1111111111111---------------------\n");////////////////////////////
	memcpy(temp,&num,sizeof(int));
	tmp = temp + sizeof(int);

	for(int n=0;n<num;n++)
	{
		if(list_head->data != NULL)
		{	
			memcpy(tmp+length*n,list_head->data,length);
		}
		list_head = list_head->next;
	}

	pack = package(head,temp);
	
	ret = write(socketfd,pack,sizeof(head)+head.length);
	if(ret < 0)
		return -1;
//	printf("--------------------------------------\n");////////////////////////////
	free(temp);
//	printf("--------------------------------------\n");////////////////////////////
	free(pack);
	return 0;
}

int packagesend(int socketfd,short type,int opt,int len,void* data,int length,int num,list_t* list_head)
{
	int ret;
	pack_head_t head;
	void *tmp,*temp;
	void* pack;

	head.type = type;
	head.opt = opt;
	head.length = length*num + sizeof(num) + len;
	
	temp = malloc(length*num + sizeof(num) + len);
	assert(temp != NULL);

//	printf("-----------------1111111111111---------------------\n");////////////////////////////
	memcpy(temp,data,len);
	memcpy(temp + len,&num,sizeof(int));
	tmp = temp + sizeof(int) + len;

	for(int n=0;n<num,list_head != NULL;n++)
	{
		if(list_head->data != NULL)
		{	
			memcpy(tmp+length*n,list_head->data,length);
		}
		list_head = list_head->next;
	}

	pack = package(head,temp);
	
	ret = write(socketfd,pack,sizeof(head)+head.length);
	if(ret < 0)
		return -1;
//	printf("--------------------------------------\n");////////////////////////////
	free(temp);
//	printf("--------------------------------------\n");////////////////////////////
	free(pack);
	return 0;
}
