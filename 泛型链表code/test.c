#include"list.h"

//以下为测试泛型链表

//数据结构
struct student
{
	int number;
	char name[20];
	float score;
};

//输入数据
void print(struct student *p)
{
	printf("%d\t%s\t%.2f\n",p->number,p->name,p->score);
	return ;
}

//按照学号查找数据
int comparenum(int *number,struct student* data)
{
	return (*number==data->number);
}

//按照姓名查找数据
int comparename(char *name,struct student *data)
{
	return (!strcmp(name,data->name));
}

//按照分数查找数据
int comparescore(float *score,struct student *data)
{
	return (*score==data->score);
}

int main()
{
	struct student s;
	struct list *p=NULL;
	struct list *head=NULL;
	int number;
	char name[20];
	float score;
	
	//创建链表
	printf("请输入学生的学号,姓名,分数:\n");
	while(1)
	{
		scanf("%d%s%f",&s.number,s.name,&s.score);
		if(s.number==0)
			break;
		p=createNode(&s,sizeof(struct student));
		head=addList(head,p);
	}
	
	//打印链表
	showList(head,(void*)print);

	//按照学号查找数据
	printf("请输入要查找的数字:\n");
	scanf("%d",&number);
	p=searchList(head,&number,(void*)comparenum);
	if(p!=NULL)
		showNode(p,(void*)print);
	else
		printf("查找的不存在\n");

	//按照姓名查找数据
	printf("请输入要查找的姓名:\n");
	scanf("%s",name);
	p=searchList(head,name,(void*)comparename);
	if(p!=NULL)
		showNode(p,(void*)print);
	else
		printf("查找的不存在\n");

	//按照分数查找数据
	printf("请输入要查找的分数:\n");
	scanf("%f",&score);
	p=searchList(head,&score,(void*)comparescore);
	if(p!=NULL)
		showNode(p,(void*)print);
	else
		printf("查找的不存在\n");

	//按照学号删除数据
	printf("请输入要删除的学号:\n");
	scanf("%d",&number);
	head=deleteList(head,&number,(void*)comparenum);
	showList(head,(void*)print);
	
	printf("链表的翻转:\n");
	head=reversal(head);
	showList(head,(void*)print);
	freeList(head);
	return 0;
}
