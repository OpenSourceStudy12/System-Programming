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
	Node* head = NULL;
	int msgid,ret;

	msgid = msgget(ftok("msg",1),IPC_CREAT|0640);
	if(msgid < 0)
	{
		perror("msg create");
		return -1;
	}

	while(1)
	{
		tmp = malloc(sizeof(Node));
		if(tmp == NULL)
		{
			fprintf(stderr,"malloc fail");
			return -1;
		}
		ret = msgrcv(msgid, &tmp->message,sizeof(Msg)-sizeof(long),1,0);//以阻塞方式接收
		if(ret < 0)
		{
			perror("message receive");
			return -1;
		}
		if(!strcmp(tmp->message.name,"NULL"))
		{
			free(tmp);
			break;
		}
		
		tmp->message.type = 1;
		tmp->next = head;
		head = tmp; 

	}

	tmp = head;
	while(tmp != NULL)
	{
		printf("name = %s,score = %.2f\n",tmp->message.name,tmp->message.score);
		tmp = tmp->next;
	}

	while(head != NULL)
	{
		tmp = head;
		head = head->next;
		free(tmp);
	}
	
	return 0;
}
