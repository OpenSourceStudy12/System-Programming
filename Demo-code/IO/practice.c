#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

typedef struct student
{
	int id;
	char name[10];
	char tel[11];
	struct student *next;
}Stu;

Stu *creat_link(Stu *head, Stu s)
{
	Stu *temp = head;
	Stu *nod = malloc(sizeof(Stu));
	nod->id = s.id;
	strcpy(nod->name, s.name);
	strcpy(nod->tel, s.tel);
	nod->next = NULL;
	
	if (head == NULL)
	{
		head = nod;
	}
	else 
	{
		while (temp->next != NULL)
			temp = temp->next;

		temp->next = nod;
	}

	return head;
}

int write_link(int fd, Stu *head)
{
	int ret;
	while (head != NULL)
	{
		ret = write(fd, head, sizeof(Stu));
		if (ret < 0)
			return -1;

		head = head->next;
	}

	return 0;
}

void free_link(Stu *head)
{
	Stu *temp = head;

	while (head != NULL)
	{
		temp = head;
		head = head->next;
		free(temp);
	}
}
int main()
{
	int fd, ret;
	Stu s;
	Stu *head = NULL;

	for (int i = 0; i < 5; i++)
	{
		printf("请输入学生的id， 姓名 电话号码:\n");
		scanf("%d %s %s", &s.id, s.name, s.tel);
		head = creat_link(head, s);
	}

	fd = open("out", O_RDWR | O_CREAT | O_TRUNC, 0640);
	if (fd < 0)
	{
		printf("打开文件失败\n");
		return -1;
	}

	ret = write_link(fd, head);
	if (ret < 0)
	{
		printf("写入失败\n");
		return -1;
	}

	lseek(fd, 0, SEEK_SET);
	while (1)
	{
		ret = read(fd, &s, sizeof(s));
		if (ret < 0)
		{
			printf("读取失败\n");
			return -1;
		}
		else if (ret == 0)
			break;

		printf("\t%d\t%s\t%s\n", s.id, s.name, s.tel);
	}

	free_link(head);
	close(fd);
	return 0;
}
