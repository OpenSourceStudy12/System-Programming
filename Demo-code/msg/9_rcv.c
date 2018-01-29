#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>

typedef struct student
{
	long type;
	int id;
	char name[20];
}STU;

typedef struct node
{
	struct student data;
	struct node *next;
}NODE;

NODE *creat_link(NODE *head, STU stu)
{
	NODE *tail = NULL;

	NODE *temp = malloc(sizeof(NODE));
	temp->data = stu;
	temp->next = NULL;

	if (head == NULL)
		head = temp;
	else 
	{
		tail = head;
		while (tail->next != NULL)
			tail = tail->next;
		tail->next = temp;
	}

	return head;
}

int write_link(NODE *head, int msgid)
{
	int ret;
	while (head != NULL)
	{
		ret = msgsnd(msgid, &(head->data), sizeof(STU) - sizeof(long), 0);
		if (ret < 0)
		{
			return -1;
		}

		head = head->next;
	}
}

void show_link(NODE *head)
{
	while (head != NULL)
	{
		printf("id = %d, name = %s\n", head->data.id, head->data.name);
		head = head->next;
	}
}

void free_link(NODE *head)
{
	NODE *temp;
	while (head != NULL)
	{
		temp = head;
		head = head->next;
		free(temp);
	}
}

int main()
{
	int msgid;
	int ret;
	STU stu;
	NODE *head = NULL;

	msgid = msgget(ftok("msg", 1), IPC_CREAT | 0640);
	if (msgid < 0)
	{
		perror("msgget");
		return -1;
	}

	for (;;)
	{
		bzero(&stu, sizeof(stu));
	//	ret = msgrcv(msgid, &stu, sizeof(stu) - sizeof(long), 1, 0);
		ret = msgrcv(msgid, &stu, sizeof(stu) - sizeof(long), 1, IPC_NOWAIT);
		if (errno == ENOMSG)
		{
			break;
		}
		else if (ret < 0)
		{
			perror("msgrcv");
			return -1;
		}

//		if (stu.id == -1)
//			break;

		head = creat_link(head, stu);
	}

	show_link(head);
	free_link(head);

	return 0;
}
