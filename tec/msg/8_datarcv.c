#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <assert.h>

struct student 
{
	long type;
	int id;
	char name[20];
	float score;
};

typedef struct node
{
	struct student data;
	struct node *next;
}Node;

Node *creat_link(Node *head, struct student stu)
{
	Node *current;

	current = malloc(sizeof(Node));
	assert(current != NULL);

	current->data = stu;
	current->next = NULL;

	if (head == NULL)
		head = current;
	else
	{
		current->next = head;
		head = current;
	}

	return head;
}

int main()
{
	int ret, msgid;
	struct student stu;
	Node *head = NULL, *temp;

	msgid = msgget(ftok("msg", 1), IPC_CREAT | 0640);
	if (msgid < 0)
	{
		perror("msgget");
		return -1;
	}

	for (;;)
	{
		ret = msgrcv(msgid, &stu, sizeof(stu)-sizeof(long), 0, IPC_NOWAIT);//以非阻塞形式接收
		if (ret < 0)
		{
			if (errno == ENOMSG)
			{
				break;
			}
			fprintf(stderr, "接受消息失败\n");
			return -1;
		}
		/*
		ret = msgrcv(msgid, &stu, sizeof(stu)-sizeof(long), 0, 0);
		if (ret < 0)
		{
			fprintf(stderr, "接受消息失败\n");
			return -1;
		}
		if (stu.id == -1)
			break;
		*/
		head = creat_link(head, stu);
	}

	temp = head;
	while (temp)
	{
		printf("id = %d, name = %s, score = %.1f\n", temp->data.id, temp->data.name, temp->data.score);
		temp = temp->next;
	}

	while (head)
	{
		temp = head;
		head = head->next;
		free(temp);
	}

	return 0;
}
