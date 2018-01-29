#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>

typedef  struct stu
{
	int num;
	char name[20];
	char sex;
	struct stu* next;
} Stu,*PStu;

void link_creat(PStu* head,Stu stu);
int link_write(int fd,PStu head);
int link_read(int fd,PStu *head);
void link_display(PStu head);
void link_free(PStu head);

int main()
{
	int fd,ret;
	Stu stu;
	PStu head1 = NULL,head2 = NULL;
	//创建链表
	for(int i=0;i<6;i++)
	{		
		scanf("%d %s %c",&stu.num,stu.name,&stu.sex);		
		link_creat(&head1,stu);
	}
	fd = open("link__",O_RDWR | O_CREAT | O_TRUNC,0640);
	if(fd < 0)
	{
		perror("open");
		return -1;
	}
	//写链表
	if(ret = link_write(fd,head1) == -1)
	{
		fprintf(stderr,"write fail\n");
		return -1;
	}
	//移动文件指针
	ret = lseek(fd,0,SEEK_SET);
	if(ret == -1)
	{
		perror("lseek");
		return -1;
	}
	//读链表
	if(ret = link_read(fd,&head2) == -1)
	{
		fprintf(stderr,"read fail\n");
		return -1;
	}
	link_display(head2);
	link_free(head1);
	link_free(head2);
	close(fd);

	return 0;
}

void link_creat(PStu* head,Stu stu)
{
	PStu ps = NULL;
	ps = malloc(sizeof(Stu));		
	ps->num = stu.num;
       	strcpy(ps->name,stu.name);
	ps->sex = stu.sex;
	ps->next = *head;
	*head = ps;
	
}

int link_write(int fd,PStu head)
{
	PStu ps = head;
	int ret;
	while(ps!=NULL)
	{
		ret = write(fd,ps,sizeof(Stu)-4);
		if(ret < 0)
		{
			return -1;
		}
		ps = ps->next;
	}
	return 0;
}

int link_read(int fd,PStu *head)
{
	PStu ps = NULL,pr;
	int ret;
	Stu stu;
	for(;;)
	{
		ret = read(fd,&stu,sizeof(Stu)-4);
		if(ret < 0)
		{
			return -1;
		}
		else if(ret == 0)
			break;
		link_creat(head,stu);
	}
}

void link_display(PStu head)
{
	while(head != NULL)
	{
		printf("%d %s %c\n",head->num,head->name,head->sex);
		head = head->next;
	}
}
void link_free(PStu head)
{
	PStu tmp;
	while(head != NULL)
	{
		tmp = head;
		head = head->next;
		free(tmp);
	}
}

