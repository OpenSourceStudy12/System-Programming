#include <stdio.h>
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

	for (int i = 0; i < 5; i++)
	{
		memset(&stu, 0, sizeof(stu));
		stu.type = 1;
		printf("请输入id:\n");
		scanf("%d", &stu.id);
		printf("请输入姓名:\n");
		scanf("%s", stu.name);
		printf("请输入得分:\n");
		scanf("%f", &stu.score);

		head = creat_link(head, stu);
	}

	temp = head;
	while (temp != NULL)
	{
		ret = msgsnd(msgid, &temp->data, sizeof(struct student)-sizeof(long), 0);
		if (ret < 0)
		{
			fprintf(stderr, "发送消息失败\n");
			return -1;
		}
		temp = temp->next;
	}

/*	stu.id = -1;
	ret = msgsnd(msgid, &stu, sizeof(stu)-sizeof(long), 0);
	if (ret < 0)
	{
		fprintf(stderr, "发送消息失败\n");
		return -1;
	}*/

	while (head)
	{
		temp = head;
		head = head->next;
		free(temp);
	}

	return 0;
}
