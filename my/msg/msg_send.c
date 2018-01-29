#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<string.h>
#include<assert.h>

typedef struct msg
{
	long type;
	char name[20];
	float score;
}Msg;

typedef struct node
{
	Msg message;
	struct node* next;
}Node;

int main()
{
	Node* tmp;
	Node* temp;
	Node* head = NULL;
	char name[20];
	float score;

	while(1)
	{
		printf("name:");
		scanf("%s",name);
		printf("score:");
		scanf("%f",&score);

		tmp = (Node*)malloc(sizeof(Node));
		assert(tmp != NULL);
		
		strcpy(tmp->message.name,name);
		tmp->message.score = score;
		tmp->message.type = 1;
		tmp->next = NULL;

		if(head == NULL)
		{
			head = tmp;
		}
		else
		{
			temp->next = tmp;
		}
		temp = tmp;

	//	tmp->next = head;
	//	head = tmp;
		if(!strcmp(name,"NULL"))
			break;
	}
	if(head == NULL)
	return -1;

	int msgid,ret;

	msgid = msgget(ftok("msg",1),IPC_CREAT|0640);
	if(msgid < 0)
	{
		perror("msg create");
		return -1;
	}
	
	tmp = head;
	while(tmp != NULL)
	{
		ret = msgsnd(msgid,&tmp->message,sizeof(Msg)-sizeof(long),0);//以阻塞方式发送
		if(ret < 0)
		{
			perror("msg send");
			return -1;
		}
		tmp = tmp->next;
	}

	while(head != NULL)
	{
		tmp = head;
		head = head->next;
		free(tmp);
	}

	printf("send message success\n");
	return 0;
}
