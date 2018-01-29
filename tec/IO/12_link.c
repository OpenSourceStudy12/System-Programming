#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct
{
	int id;
	char name[200];
	char sex;
}STU;

struct Node
{
	STU stu;
	struct Node *next;
};

struct Node *create_link(struct Node *head, STU stu)
{
	struct Node *current = malloc(sizeof(struct Node));
//	assert(current != NULL);
	struct Node *temp;

	current->stu = stu;
	current->next = NULL;

	//尾插法
	if (head == NULL)
	{
		head = current;
	}
	else 
	{
		temp = head;
		while (temp->next != NULL)
			temp = temp->next;

		temp->next = current;
	}

	return head;
}

int write_link(int fd, struct Node *head)
{
	int ret;
	while (head)
	{
		ret = write(fd, &(head->stu), sizeof(STU));
		if (ret < 0)
			return -1;

		head = head->next;
	}
	return 0;
}

struct Node *read_link(int fd, struct Node *head)
{
	int ret;
	STU stu;

	for (;;)
	{
		ret = read(fd, &stu, sizeof(stu));
		if (ret < 0)
			return NULL;
		else if (ret == 0)
			break;

		head = create_link(head, stu);
	}
	return head;
}

void free_link(struct Node *head)
{
	struct Node *temp;
	while (head != NULL)
	{
		temp = head;
		head = head->next;
		free(temp);
	}
}

void show_link(struct Node *head)
{
	while (head)
	{
		printf("%d\t%s\t%c\n", head->stu.id, head->stu.name, head->stu.sex);
		head = head->next;
	}
}

int main()
{
	int fd, ret;
	STU stu;
	struct Node *head = NULL;

	fd = open("io", O_RDWR | O_CREAT | O_TRUNC, 0640);
	if (fd < 0)
	{
		perror("open");
		return -1;
	}

	for (int i = 0; i < 5; i++)
	{
		printf("请输入学号，姓名，性别：\n");
		scanf("%d %s %c", &stu.id, stu.name, &stu.sex);
		head = create_link(head, stu);
	}

	ret = write_link(fd, head);
	if (ret < 0)
	{
		fprintf(stderr, "write fail\n");
		return -1;
	}

	free_link(head);
	head = NULL;

	lseek(fd, 0, SEEK_SET);
	
	head = read_link(fd, head);
	if (head == 0)
	{
		fprintf(stderr, "read fail\n");
		return -1;
	}
	
	show_link(head);
	free_link(head);
	close(fd);
	return 0;
}
